// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Compositor/Compositor.h"
#include "Core/AutoLock.h"
#include "Core/Guard.h"

#include <cassert>

using namespace rl;

Compositor::Compositor(std::shared_ptr<RenderSurface> surface)
    : _surface(surface), _looper(nullptr), _lock() {
  assert(_surface != nullptr && "A surface must be provided to the compositor");
  _surface->setObserver(this);
}

Compositor::~Compositor() {
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
  _looper->dispatchAsync([] {});
}

void Compositor::surfaceSizeUpdated(double width, double height) {
  _looper->dispatchAsync([] {});
}

void Compositor::surfaceWasDestroyed() {
  _looper->dispatchAsync([] {});
}
