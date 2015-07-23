// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Compositor/Compositor.h"
#include "Core/AutoLock.h"
#include "Core/Guard.h"

#include <cassert>

using namespace rl;

Compositor::Compositor(std::shared_ptr<RenderSurface> surface)
    : _surface(surface),
      _looper(nullptr),
      _lock(),
      _vsyncSource(LooperSource::AsTimer(std::chrono::milliseconds(16))),
      _surfaceSize(SizeZero) {
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

  _looper->loop(ready);
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
}

void Compositor::stopComposition() {
  _looper->removeSource(_vsyncSource);
}

void Compositor::onVsync() {
  bool res = _surface->makeCurrent();
  assert(res && "Must be able to make the current context current");

  res = _surface->present();
  assert(res && "Must be able to present the current context");
}
