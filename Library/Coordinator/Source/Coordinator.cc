// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Coordinator/Coordinator.h>
#include <Compositor/Frame.h>
#include <Core/TraceEvent.h>
#include <Coordinator/RenderSurface.h>
#include <Compositor/BackendPass.h>

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
  auto controllersAccess = _interfaceControllers.access();

  if (_surfaceSize == size) {
    return;
  }

  _surfaceSize = size;

  for (auto& controller : controllersAccess.get()) {
    controller.setSize(_surfaceSize);
  }
}

void Coordinator::renderSurfaceWasTornDown() {
  /*
   *  This method may be called on any thread.
   */
}

void Coordinator::setupOrTeardownChannels(bool setup) {
  auto controllers = _interfaceControllers.access();
  if (setup) {
    scheduleInterfaceChannels(true, controllers);
    _loop->addSource(_animationsSource);
    _loop->addSource(_coordinatorAcquisitionProtocol.source());
  } else {
    scheduleInterfaceChannels(false, controllers);
    _loop->removeSource(_animationsSource);
    _loop->removeSource(_coordinatorAcquisitionProtocol.source());
  }
}

CoordinatorAcquisitionProtocol::VendorResult
Coordinator::acquireFreshCoordinatorChannel() {
  auto controllersAccess = _interfaceControllers.access();

  /*
   *  Create a new interface controller for this reques
   */
  controllersAccess.get().emplace_back(_interfaceTagGenerator.acquire(),
                                       _surfaceSize);

  /*
   *  Schedule all channels
   */
  scheduleInterfaceChannels(true, controllersAccess);

  /*
   *  Return the vendor result to the protocol
   */
  auto& controller = controllersAccess.get().back();
  return CoordinatorAcquisitionProtocol::VendorResult(controller.channel(),
                                                      controller.debugTag());
}

void Coordinator::scheduleInterfaceChannels(
    bool schedule,
    InterfaceControllers::Access& controllers) {
  if (_loop == nullptr) {
    return;
  }

  for (auto& controller : controllers.get()) {
    controller.scheduleChannel(*_loop, schedule);
  }
}

void Coordinator::updateAndRenderInterfaceControllers(bool force) {
  RL_TRACE_INSTANT(__function__);
  RL_TRACE_AUTO(__function__);

  auto controllersAccess = _interfaceControllers.access();

  auto touchesIfAny = _touchEventChannel.drainPendingTouches();

  auto wasUpdated = false;

  for (auto& controller : controllersAccess.get()) {
    wasUpdated |= controller.update(touchesIfAny);
  }

  if (wasUpdated || _forceAnotherFrame || force) {
    /*
     *  If the scene was updated but the frame could not be rendered (for
     *  whatever reason), force the next frame.
     */
    _forceAnotherFrame = !renderSingleFrame(controllersAccess);
  }
}

void Coordinator::redrawCurrentFrameNow() {
  updateAndRenderInterfaceControllers(true);
}

bool Coordinator::renderSingleFrame(InterfaceControllers::Access& controllers) {
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

  for (auto& interface : controllers.get()) {
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
    surfaceAccess.finalizeForPresentation();
  }

  return frame.end();
}

}  // namespace coordinator
}  // namespace rl
