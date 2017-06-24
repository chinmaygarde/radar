/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Compositor/BackendPass.h>
#include <Compositor/Frame.h>
#include <Coordinator/Coordinator.h>
#include <Coordinator/RenderSurface.h>
#include <Core/TraceEvent.h>

namespace rl {
namespace coordinator {

Coordinator::Coordinator(std::shared_ptr<RenderSurface> surface,
                         event::TouchEventChannel& touchEventChannel)
    : _surface(surface),
      _loop(nullptr),
      _interfaceTagGenerator("rl.interface"),
      _animationsSource(core::EventLoopSource::Timer(
          std::chrono::duration_cast<core::ClockDurationNano>(
              core::ClockDurationSeconds(1.0 / 60.0)))),
      _touchEventChannel(touchEventChannel),
      _coordinatorAcquisitionProtocol(
          std::bind(&Coordinator::acquireFreshCoordinatorChannel, this)),
      _forceAnotherFrame(true) {
  RL_ASSERT_MSG(_surface != nullptr,
                "A surface must be provided to the coordinator");
  _animationsSource->setWakeFunction(std::bind(
      &Coordinator::updateAndRenderInterfaceControllers, this, false));
}

Coordinator::~Coordinator() = default;

void Coordinator::run(std::function<void()> onReady) {
  if (_loop != nullptr) {
    if (onReady) {
      onReady();
    }
    return;
  }

  _loop = core::EventLoop::Current();

  setupOrTeardownChannels(true);

  _loop->loop(onReady);
}

void Coordinator::shutdown(std::function<void()> onShutdown) {
  if (_loop == nullptr) {
    if (onShutdown) {
      onShutdown();
    }
    return;
  }

  _loop->dispatchAsync([&] {
    setupOrTeardownChannels(false);
    auto disposed = _context.dispose();
    RL_ASSERT_MSG(disposed, "Must be able to dispose the compositor context.");
    _loop->terminate();
    if (onShutdown) {
      onShutdown();
    }
  });
}

bool Coordinator::isRunning() const {
  return _loop != nullptr;
}

void Coordinator::renderSurfaceWasSetup() {
  /*
   *  This method may be called on any thread.
   */
}

void Coordinator::renderSurfaceSizeUpdated(const geom::Size& size) {
  if (_surfaceSize == size) {
    return;
  }

  _surfaceSize = size;

  core::MutexLocker lock(_interfaceControllersMutex);
  for (auto& controller : _interfaceControllers) {
    controller.setSize(_surfaceSize);
  }
}

void Coordinator::renderSurfaceWasTornDown() {
  /*
   *  This method may be called on any thread.
   */
}

void Coordinator::setupOrTeardownChannels(bool setup) {
  core::MutexLocker lock(_interfaceControllersMutex);
  if (setup) {
    scheduleInterfaceChannels(true);
    _loop->addSource(_animationsSource);
    _loop->addSource(_coordinatorAcquisitionProtocol.source());
  } else {
    scheduleInterfaceChannels(false);
    _loop->removeSource(_animationsSource);
    _loop->removeSource(_coordinatorAcquisitionProtocol.source());
  }
}

CoordinatorAcquisitionProtocol::VendorResult
Coordinator::acquireFreshCoordinatorChannel() {
  core::MutexLocker lock(_interfaceControllersMutex);
  /*
   *  Create a new interface controller for this reques
   */
  _interfaceControllers.emplace_back(_interfaceTagGenerator.acquire(),
                                     _surfaceSize);

  /*
   *  Schedule all channels
   */
  scheduleInterfaceChannels(true);

  /*
   *  Return the vendor result to the protocol
   */
  auto& controller = _interfaceControllers.back();
  return CoordinatorAcquisitionProtocol::VendorResult(controller.channel(),
                                                      controller.debugTag());
}

void Coordinator::scheduleInterfaceChannels(bool schedule) {
  if (_loop == nullptr) {
    return;
  }

  for (auto& controller : _interfaceControllers) {
    controller.scheduleChannel(*_loop, schedule);
  }
}

void Coordinator::updateAndRenderInterfaceControllers(bool force) {
  RL_TRACE_INSTANT(__function__);
  RL_TRACE_AUTO(__function__);

  auto touchesIfAny = _touchEventChannel.drainPendingTouches();

  const bool consoleInterceptsTouches =
      _context.applyTouchesToConsole(touchesIfAny);

  bool wasUpdated = consoleInterceptsTouches;

  core::MutexLocker lock(_interfaceControllersMutex);

  for (auto& controller : _interfaceControllers) {
    wasUpdated |= controller.update(touchesIfAny);
  }

  if (wasUpdated || _forceAnotherFrame || force) {
    /*
     *  If the scene was updated but the frame could not be rendered (for
     *  whatever reason), force the next frame.
     */
    _forceAnotherFrame = !renderSingleFrame();
  }
}

void Coordinator::redrawCurrentFrameNow() {
  updateAndRenderInterfaceControllers(true);
}

bool Coordinator::renderSingleFrame() {
  RL_TRACE_AUTO(__function__);

  if (_surfaceSize.width <= 0.0 || _surfaceSize.height <= 0.0) {
    /*
     *  Don't bother the platform if the surface size is zero.
     */
    return true;
  }

  /*
   *  A single backend pass is created for all registered interface controller.
   *  The result of rendering of each interface controller is a discrete
   *  front-end pass.
   */
  compositor::BackEndPass backEndPass;

  for (auto& interface : _interfaceControllers) {
    backEndPass.addFrontEndPass(interface.render());
  }

  if (!backEndPass.hasRenderables()) {
    /*
     *  Don't bother acquiring the surface is there are no renderables.
     */
    return true;
  }

  ScopedRenderSurfaceAccess surfaceAccess(*_surface);

  if (!surfaceAccess.acquired()) {
    return false;
  }

  compositor::Frame frame(_surfaceSize, _context);

  if (!frame.isReady()) {
    return false;
  }

  if (!frame.begin()) {
    return false;
  }

  if (backEndPass.render(frame, &_workQueue)) {
    /*
     *  Render statistics for each interface controller.
     */
    for (const auto& interface : _interfaceControllers) {
      interface.presentStatistics();
    }
    surfaceAccess.finalizeForPresentation();
  }

  return frame.end();
}

}  // namespace coordinator
}  // namespace rl
