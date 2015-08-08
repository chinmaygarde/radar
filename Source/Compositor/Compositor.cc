// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Compositor/Compositor.h"
#include "Compositor/Frame.h"

#include <cassert>

namespace rl {

Compositor::Compositor(std::shared_ptr<RenderSurface> surface)
    : _surface(surface),
      _looper(nullptr),
      _lock(),
      _vsyncSource(LooperSource::AsTimer(std::chrono::milliseconds(16))),
      _surfaceSize(SizeZero),
      _rootLayer(nullptr),
      _programCatalog(nullptr),
      _patchChannel(std::make_shared<Channel>()) {
  assert(_surface != nullptr && "A surface must be provided to the compositor");

  _surface->setObserver(this);
  _vsyncSource->setWakeFunction([&] { onVsync(); });
}

Compositor::~Compositor() {
  _vsyncSource->setWakeFunction(nullptr);
  _surface->setObserver(nullptr);
}

void Compositor::run(Latch& readyLatch) {
  auto ready = [&readyLatch]() { readyLatch.countDown(); };

  if (_looper != nullptr) {
    ready();
    return;
  }

  _looper = Looper::Current();
  setupChannels();
  _looper->loop(ready);
}

void Compositor::shutdown(Latch& shutdownLatch) {
  if (_looper == nullptr) {
    shutdownLatch.countDown();
    return;
  }

  _looper->dispatchAsync([&] {
    teardownChannels();
    stopComposition();
    _looper->terminate();
    shutdownLatch.countDown();
  });
}

bool Compositor::isRunning() const {
  return _looper != nullptr;
}

void Compositor::surfaceWasCreated() {
  _looper->dispatchAsync([&] { startComposition(); });
}

void Compositor::surfaceSizeUpdated(const Size& size) {
  _looper->dispatchAsync([&, size] { commitCompositionSizeUpdate(size); });
}

void Compositor::surfaceWasDestroyed() {
  _looper->dispatchAsync([&] { stopComposition(); });
}

void Compositor::startComposition() {
  _looper->addSource(_vsyncSource);
}

void Compositor::commitCompositionSizeUpdate(const Size& size) {
  if (size == _surfaceSize) {
    return;
  }

  // Commit size update
  _surfaceSize = size;
}

void Compositor::stopComposition() {
  _looper->removeSource(_vsyncSource);
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

  // Setup catalog

  return _programCatalog;
}

void Compositor::drawFrame() {
  Frame frame(_surfaceSize, accessCatalog());

  frame.begin();

  if (_rootLayer) {
    _rootLayer->drawInFrame(frame);
  }

  frame.end();
}

void Compositor::onVsync() {
  bool res = _surface->makeCurrent();
  assert(res && "Must be able to make the current context current");
  drawFrame();

  res = _surface->present();
  assert(res && "Must be able to present the current context");
}

void Compositor::setupChannels() {
  bool res = _looper->addSource(_patchChannel->source());
  assert(res == true);
}

void Compositor::teardownChannels() {
  bool res = _looper->removeSource(_patchChannel->source());
  assert(res == true);
}

std::weak_ptr<Channel> Compositor::patchChannel() {
  return _patchChannel;
}

}  // namespace rl
