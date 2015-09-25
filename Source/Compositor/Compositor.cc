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
      _interfaceChannel(nullptr),
      _animationsSource(EventLoopSource::Timer(ClockDurationGod)) {
  RL_ASSERT(_surface != nullptr &&
            "A surface must be provided to the compositor");
  _surface->setObserver(this);
  _animationsSource->setWakeFunction(
      std::bind(&Compositor::onAnimationsFlush, this));
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
  _loop->addSource(_animationsSource);
}

void Compositor::teardownChannels() {
  manageInterfaceUpdates(false);
  _loop->removeSource(_animationsSource);
}

std::weak_ptr<CompositorChannel> Compositor::acquireChannel() {
  if (_interfaceChannel != nullptr) {
    return _interfaceChannel;
  }

  _interfaceChannel = std::make_shared<CompositorChannel>();
  manageInterfaceUpdates(true);

  RL_ASSERT(_interfaceChannel != nullptr);
  return _interfaceChannel;
}

void Compositor::manageInterfaceUpdates(bool schedule) {
  if (_loop == nullptr || _interfaceChannel == nullptr) {
    return;
  }

  auto source = _interfaceChannel->source();

  if (schedule) {
    _interfaceChannel->setMessagesReceivedCallback([&](Messages messages) {
      if (applyTransactionMessages(std::move(messages))) {
        drawSingleFrame();
      }
    });
    _loop->addSource(source);
  } else {
    _interfaceChannel->setMessagesReceivedCallback(nullptr);
    _loop->removeSource(source);
  }
}

void Compositor::onAnimationsFlush() {
  auto flushed =
      _graph.animationDirector().flushInterpolations(_stats.interpolations());
  if (flushed > 0) {
    drawSingleFrame();
    _stats.interpolationsCount().reset(flushed);
  }
}

bool Compositor::applyTransactionMessages(Messages messages) {
  AutoStopwatchLap transactionUpdateTimer(_stats.transactionUpdateTimer());
  bool result = true;
  for (auto& message : messages) {
    result &= _graph.applyTransactions(message);
  }
  return result;
}

}  // namespace rl
