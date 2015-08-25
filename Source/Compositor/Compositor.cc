// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/Compositor.h>
#include <Compositor/Frame.h>

namespace rl {

Compositor::Compositor(std::shared_ptr<RenderSurface> surface)
    : _surface(surface),
      _loop(nullptr),
      _lock(),
      _surfaceSize(SizeZero),
      _programCatalog(nullptr),
      _lease(nullptr),
      _graph(),
      _stats(),
      _statsRenderer() {
  RL_ASSERT(_surface != nullptr &&
            "A surface must be provided to the compositor");
  _surface->setObserver(this);
}

Compositor::~Compositor() {
  _surface->setObserver(nullptr);
}

void Compositor::run(Latch& readyLatch) {
  auto ready = [&readyLatch]() { readyLatch.countDown(); };

  if (_loop != nullptr) {
    ready();
    return;
  }

  _loop = EventLoop::Current();
  setupChannels();
  _loop->loop(ready);
}

void Compositor::shutdown(Latch& shutdownLatch) {
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

bool Compositor::isRunning() const {
  return _loop != nullptr;
}

void Compositor::surfaceWasCreated() {
  _loop->dispatchAsync([&] { startComposition(); });
}

void Compositor::surfaceSizeUpdated(const Size& size) {
  _loop->dispatchAsync([&, size] { commitCompositionSizeUpdate(size); });
}

void Compositor::surfaceWasDestroyed() {
  _loop->dispatchAsync([&] { stopComposition(); });
}

void Compositor::startComposition() {
  drawSingleFrame();
}

void Compositor::stopComposition() {
}

void Compositor::commitCompositionSizeUpdate(const Size& size) {
  if (size == _surfaceSize) {
    return;
  }

  /*
   *  Commit size update
   */
  _surfaceSize = size;
}

/**
 *  Initializes and returns the compositor program catalog. Must be accessed
 *  on the compositor thread
 *
 *  @return the program catalog
 */
std::shared_ptr<ProgramCatalog> Compositor::accessCatalog() {
  if (_programCatalog != nullptr) {
    return _programCatalog;
  }

  _programCatalog = std::make_shared<ProgramCatalog>();

  /*
   *  Setup catalog
   */
  return _programCatalog;
}

void Compositor::drawSingleFrame() {
  CompositorStatisticsFrame statistics(_stats);

  ScopedRenderSurfaceAccess access(*_surface);
  ScopedFrame frame(_surfaceSize, accessCatalog(), _stats);

  if (!frame.isReady()) {
    return;
  }

  _stats.frameCount().increment();

  _graph.render(frame);

  _statsRenderer.render(_stats, frame);
}

void Compositor::setupChannels() {
  manageInterfaceUpdates(true);
}

void Compositor::teardownChannels() {
  manageInterfaceUpdates(false);
}

EntityLease& Compositor::acquireLease(size_t count) {
  if (_lease != nullptr) {
    return *_lease;
  }

  _lease = rl::make_unique<EntityLease>(count);
  manageInterfaceUpdates(true);

  RL_ASSERT(_lease != nullptr);
  return *_lease;
}

void Compositor::manageInterfaceUpdates(bool schedule) {
  if (_loop == nullptr || _lease == nullptr) {
    return;
  }

  if (schedule) {
    auto source = _lease->writeNotificationSource();
    source->setWakeFunction(std::bind(&Compositor::onInterfaceDidUpdate, this));
    _loop->addSource(source);
  } else {
    _loop->removeSource(_lease->writeNotificationSource());
  }
}

void Compositor::onInterfaceDidUpdate() {
  if (_lease == nullptr) {
    return;
  }

  _graph.applyUpdates(_lease->swapReadArena());
  drawSingleFrame();
}

}  // namespace rl
