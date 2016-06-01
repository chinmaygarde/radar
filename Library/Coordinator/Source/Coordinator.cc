// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Coordinator/Coordinator.h>
#include <Compositor/Frame.h>
#include <Core/TraceEvent.h>
#include <Coordinator/RenderSurface.h>

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
  std::lock_guard<std::mutex> lock(_interfaceControllersMutex);

  if (_surfaceSize == size) {
    return;
  }

  _surfaceSize = size;

  for (auto& interfaceController : _interfaceControllers) {
    interfaceController.updateSize(_surfaceSize);
  }
}

void Coordinator::renderSurfaceWasTornDown() {
  /*
   *  This method may be called on any thread.
   */
}

void Coordinator::setupOrTeardownChannels(bool setup) {
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
  std::lock_guard<std::mutex> lock(_interfaceControllersMutex);

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

  for (auto& interface : _interfaceControllers) {
    interface.scheduleChannel(*_loop, schedule);
  }
}

void Coordinator::updateAndRenderInterfaceControllers(bool force) {
  RL_TRACE_INSTANT("UpdateAndRenderInterfaceControllers");
  RL_TRACE_AUTO("UpdateAndRenderInterfaceControllers");

  std::lock_guard<std::mutex> lock(_interfaceControllersMutex);

  auto touchesIfAny = _touchEventChannel.drainPendingTouches();

  auto wasUpdated = false;

  for (auto& interface : _interfaceControllers) {
    wasUpdated |= interface.updateInterface(touchesIfAny);
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
  RL_TRACE_AUTO("RenderFrame");

  if (_surfaceSize.width <= 0.0 || _surfaceSize.height <= 0.0) {
    /*
     *  Don't bother the platform if the surface size is zero. Just say we have
     *  rendered and go on with it.
     */
    return true;
  }

  ScopedRenderSurfaceAccess surfaceAccess(*_surface);

  if (!surfaceAccess.acquired()) {
    return false;
  }

  compositor::ScopedFrame frame(_surfaceSize, _context);

  if (!frame.isReady()) {
    return false;
  }

  auto wasRendered = false;

  for (auto& interface : _interfaceControllers) {
    wasRendered |= interface.renderCurrentInterfaceState(frame);
  }

  if (wasRendered) {
    surfaceAccess.finalizeForPresentation();
  }

  return true;
}

}  // namespace coordinator
}  // namespace rl
