// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Coordinator/Coordinator.h>
#include <Coordinator/Frame.h>

namespace rl {
namespace coordinator {

Coordinator::Coordinator(std::shared_ptr<RenderSurface> surface,
                         event::TouchEventChannel& touchEventChannel)
    : _surface(surface),
      _loop(nullptr),
      _animationsSource(core::EventLoopSource::Timer(core::ClockDurationGod)),
      _touchEventChannel(touchEventChannel) {
  RL_ASSERT_MSG(_surface != nullptr,
                "A surface must be provided to the coordinator");
  _surface->setObserver(this);
  _animationsSource->setWakeFunction(
      std::bind(&Coordinator::onDisplayLinkPulse, this));
}

Coordinator::~Coordinator() {
  _surface->setObserver(nullptr);
}

void Coordinator::run(core::Latch& readyLatch) {
  auto ready = [&readyLatch]() { readyLatch.countDown(); };

  if (_loop != nullptr) {
    ready();
    return;
  }

  _loop = core::EventLoop::Current();
  setupChannels();
  _loop->loop(ready);
}

void Coordinator::shutdown(core::Latch& shutdownLatch) {
  if (_loop == nullptr) {
    shutdownLatch.countDown();
    return;
  }

  _loop->dispatchAsync([&] {
    teardownChannels();
    stopComposition();
    _loop->terminate();
    shutdownLatch.countDown();
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
  // TODO: Prepare for removal
}

void Coordinator::stopComposition() {
  // TODO: Prepare for removal
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

void Coordinator::setupChannels() {
  scheduleInterfaceChannels(true);
  _loop->addSource(_animationsSource);
}

void Coordinator::teardownChannels() {
  scheduleInterfaceChannels(false);
  _loop->removeSource(_animationsSource);
}

std::weak_ptr<Channel> Coordinator::acquireChannel() {
  if (_interfaces.size() > 0) {
    return _interfaces.front().channel();
  }

  _interfaces.emplace_back();
  scheduleInterfaceChannels(true);

  return _interfaces.front().channel();
}

void Coordinator::scheduleInterfaceChannels(bool schedule) {
  if (_loop == nullptr) {
    return;
  }

  for (auto& interface : _interfaces) {
    interface.scheduleChannel(*_loop, schedule);
  }
}

void Coordinator::onDisplayLinkPulse() {
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

}  // namespace coordinator
}  // namespace rl
