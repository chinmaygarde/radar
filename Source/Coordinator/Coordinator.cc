// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Coordinator/Coordinator.h>
#include <Coordinator/Frame.h>
#include <Instrumentation/TraceEvent.h>

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
      _interfaceAcquisitionProtocol(
          std::bind(&Coordinator::acquireFreshInterfaceChannel, this)) {
  RL_ASSERT_MSG(_surface != nullptr,
                "A surface must be provided to the coordinator");
  _surface->setObserver(this);

  _animationsSource->setWakeFunction(
      std::bind(&Coordinator::onDisplayLink, this));
}

Coordinator::~Coordinator() {
  _surface->setObserver(nullptr);
}

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
    stopComposition();
    _loop->terminate();
    if (onShutdown) {
      onShutdown();
    }
  });
}

bool Coordinator::isRunning() const {
  return _loop != nullptr;
}

void Coordinator::surfaceWasCreated() {
  _loop->dispatchAsync([&] { startComposition(); });
}

void Coordinator::surfaceSizeUpdated(const geom::Size& size) {
  _loop->dispatchAsync([&, size] { commitCompositionSizeUpdate(size); });
}

void Coordinator::surfaceWasDestroyed() {
  _loop->dispatchAsync([&] { stopComposition(); });
}

void Coordinator::startComposition() {
  //
}

void Coordinator::stopComposition() {
  //
}

void Coordinator::commitCompositionSizeUpdate(const geom::Size& size) {
  if (size == _surfaceSize) {
    return;
  }

  /*
   *  Commit size update
   */
  _surfaceSize = size;
}

/**
 *  Initializes and returns the coordinator program catalog. Must be accessed
 *  on the coordinator thread
 *
 *  @return the program catalog
 */
std::shared_ptr<ProgramCatalog> Coordinator::accessCatalog() {
  if (_programCatalog != nullptr) {
    return _programCatalog;
  }

  _programCatalog = std::make_shared<ProgramCatalog>();

  /*
   *  Setup catalog
   */
  return _programCatalog;
}

void Coordinator::setupOrTeardownChannels(bool setup) {
  if (setup) {
    scheduleInterfaceChannels(true);
    _loop->addSource(_animationsSource);
    _loop->addSource(_interfaceAcquisitionProtocol.source());
  } else {
    scheduleInterfaceChannels(false);
    _loop->removeSource(_animationsSource);
    _loop->removeSource(_interfaceAcquisitionProtocol.source());
  }
}

InterfaceAcquisitionProtocol::VendorResult
Coordinator::acquireFreshInterfaceChannel() {
  /*
   *  Create a new interface controller for this reques
   */
  _interfaces.emplace_back(_interfaceTagGenerator.acquire());

  /*
   *  Schedule all channels
   */
  scheduleInterfaceChannels(true);

  /*
   *  Return the vendor result to the protocol
   */
  auto& interface = _interfaces.back();
  return InterfaceAcquisitionProtocol::VendorResult(interface.channel(),
                                                    interface.debugTag());
}

void Coordinator::scheduleInterfaceChannels(bool schedule) {
  if (_loop == nullptr) {
    return;
  }

  for (auto& interface : _interfaces) {
    interface.scheduleChannel(*_loop, schedule);
  }
}

void Coordinator::onDisplayLink() {
  RL_TRACE_INSTANT("OnDisplayLink");
  RL_TRACE_AUTO("OnDisplayLink");

  auto touchesIfAny = _touchEventChannel.drainPendingTouches();

  auto wasUpdated = false;

  for (auto& interface : _interfaces) {
    wasUpdated |= interface.updateInterface(touchesIfAny);
  }

  if (wasUpdated) {
    renderFrame();
  }
}

void Coordinator::renderFrame() {
  RL_TRACE_AUTO("RenderFrame");

  ScopedRenderSurfaceAccess surfaceAccess(*_surface);

  if (!surfaceAccess.acquired()) {
    return;
  }

  ScopedFrame frame(_surfaceSize, accessCatalog(), _stats);

  if (!frame.isReady()) {
    return;
  }

  StatisticsFrame statistics(_stats);

  _stats.frameCount().increment();

  bool wasRendered = false;

  for (auto& interface : _interfaces) {
    wasRendered |= interface.renderCurrentInterfaceState(frame);
  }

  if (wasRendered) {
    _statsRenderer.render(_stats, frame);
    surfaceAccess.finalizeForPresentation();
  }
}

const char* CoordinatorInterfaceChannelVendorName =
    "com.radarlove.coordinator.interface";

}  // namespace coordinator
}  // namespace rl
