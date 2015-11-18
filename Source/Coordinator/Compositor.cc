// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Coordinator/Compositor.h>
#include <Coordinator/Frame.h>

namespace rl {
namespace coordinator {

Compositor::Compositor(std::shared_ptr<RenderSurface> surface,
                       event::TouchEventChannel& touchEventChannel)
    : _surface(surface),
      _loop(nullptr),
      _surfaceSize(geom::SizeZero),
      _programCatalog(nullptr),
      _interfaceChannel(nullptr),
      _animationsSource(core::EventLoopSource::Timer(core::ClockDurationGod)),
      _touchEventChannel(touchEventChannel) {
  RL_ASSERT_MSG(_surface != nullptr,
                "A surface must be provided to the compositor");
  _surface->setObserver(this);
  _animationsSource->setWakeFunction(
      std::bind(&Compositor::onAnimationsStep, this));
}

Compositor::~Compositor() {
  _surface->setObserver(nullptr);
}

void Compositor::run(core::Latch& readyLatch) {
  auto ready = [&readyLatch]() { readyLatch.countDown(); };

  if (_loop != nullptr) {
    ready();
    return;
  }

  _loop = core::EventLoop::Current();
  setupChannels();
  _loop->loop(ready);
}

void Compositor::shutdown(core::Latch& shutdownLatch) {
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

void Compositor::surfaceSizeUpdated(const geom::Size& size) {
  _loop->dispatchAsync([&, size] { commitCompositionSizeUpdate(size); });
}

void Compositor::surfaceWasDestroyed() {
  _loop->dispatchAsync([&] { stopComposition(); });
}

void Compositor::startComposition() {
  prepareSingleFrame();
}

void Compositor::stopComposition() {
}

void Compositor::commitCompositionSizeUpdate(const geom::Size& size) {
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

void Compositor::setupChannels() {
  manageInterfaceUpdates(true);
  _loop->addSource(_animationsSource);
}

void Compositor::teardownChannels() {
  manageInterfaceUpdates(false);
  _loop->removeSource(_animationsSource);
}

std::weak_ptr<Channel> Compositor::acquireChannel() {
  if (_interfaceChannel != nullptr) {
    return _interfaceChannel;
  }

  _interfaceChannel = std::make_shared<Channel>();
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
    namespace P = std::placeholders;
    _interfaceChannel->setMessagesReceivedCallback(
        std::bind(&Compositor::onInterfaceTransactionUpdate, this, P::_1));
    _loop->addSource(source);
  } else {
    _interfaceChannel->setMessagesReceivedCallback(nullptr);
    _loop->removeSource(source);
  }
}

bool Compositor::applyTransactionMessages(core::Messages messages) {
  instrumentation::AutoStopwatchLap transactionUpdateTimer(
      _stats.transactionUpdateTimer());
  bool result = true;
  for (auto& message : messages) {
    result &= _graph.applyTransactions(message);
  }
  return result;
}

void Compositor::onInterfaceTransactionUpdate(core::Messages messages) {
  if (applyTransactionMessages(std::move(messages))) {
    prepareSingleFrame();
  }
}

void Compositor::onAnimationsStep() {
  const auto count =
      _graph.animationDirector().stepInterpolations(_stats.interpolations());
  if (count > 0) {
    prepareSingleFrame();
    _stats.interpolationsCount().reset(count);
  }
}

void Compositor::prepareSingleFrame() {
  drainPendingTouches();
  drawSingleFrame();
}

void Compositor::drawSingleFrame() {
  StatisticsFrame statistics(_stats);

  ScopedRenderSurfaceAccess access(*_surface);

  if (!access.acquired()) {
    return;
  }

  ScopedFrame frame(_surfaceSize, accessCatalog(), _stats);

  if (!frame.isReady()) {
    return;
  }

  _stats.frameCount().increment();

  _graph.render(frame);

  _statsRenderer.render(_stats, frame);

  access.finalize();
}

void Compositor::drainPendingTouches() {
  auto touchMap = _touchEventChannel.drainPendingTouches();

  if (touchMap.size() == 0) {
    return;
  }

  auto res = _graph.applyTouchMap(std::move(touchMap));
  RL_ASSERT(res);
}

}  // namespace coordinator
}  // namespace rl
