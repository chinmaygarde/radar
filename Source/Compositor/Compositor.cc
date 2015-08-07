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
      _programCatalog(nullptr) {
  assert(_surface != nullptr && "A surface must be provided to the compositor");

  _surface->setObserver(this);
  _vsyncSource->setWakeFunction([&] { onVsync(); });

  // Test Layers
  _rootLayer = std::make_shared<Layer>();
  _rootLayer->setBackgroundColor(ColorRed);
  _rootLayer->setFrame(Rect(200, 200, 300, 200));

  auto sublayer = Layer::Ref::make_shared();
  sublayer->setFrame({10, 10, 100, 300});
  sublayer->setBackgroundColor(ColorGreen);
  _rootLayer->addSublayer(sublayer);

  auto sublayer2 = Layer::Ref::make_shared();
  sublayer2->setFrame({10, 10, 75, 75});
  sublayer2->setBackgroundColor(ColorBlue);
  sublayer->addSublayer(sublayer2);
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

  _looper->loop(ready);
}

void Compositor::shutdown(Latch& shutdownLatch) {
  if (_looper == nullptr) {
    shutdownLatch.countDown();
    return;
  }

  _looper->dispatchAsync([&] {
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

void Compositor::surfaceSizeUpdated(double width, double height) {
  _looper->dispatchAsync(
      [&, width, height] { commitCompositionSizeUpdate(width, height); });
}

void Compositor::surfaceWasDestroyed() {
  _looper->dispatchAsync([&] { stopComposition(); });
}

void Compositor::startComposition() {
  _looper->addSource(_vsyncSource);
}

void Compositor::commitCompositionSizeUpdate(double width, double height) {
  Size size(width, height);

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
  if (_rootLayer == nullptr) {
    return;
  }

  Frame frame(_surfaceSize, accessCatalog());

  frame.begin();

  _rootLayer->drawInFrame(frame);

  frame.end();
}

void Compositor::onVsync() {
  bool res = _surface->makeCurrent();
  assert(res && "Must be able to make the current context current");

  drawFrame();

  res = _surface->present();
  assert(res && "Must be able to present the current context");
}

}  // namespace rl
