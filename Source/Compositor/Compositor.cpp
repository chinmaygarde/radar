// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Compositor/Compositor.h"
#include "Core/AutoLock.h"
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

void Compositor::run() {
  if (_looper != nullptr) {
    return;
  }

  _looper = Looper::Current();
  _looper->loop();
}

void Compositor::surfaceWasCreated() {
  // Needs to be dispatched to the compositors looper
  assert(_looper && "Looper must be ready");
  _looper->dispatchAsync([] { printf("Hello 1"); });
}

void Compositor::surfaceSizeUpdated(double width, double height) {
  // Needs to be dispatched to the compositors looper
  assert(_looper && "Looper must be ready");
  _looper->dispatchAsync([] { printf("Hello 2"); });
}

void Compositor::surfaceWasDestroyed() {
  // Needs to be dispatched to the compositors looper
  assert(_looper && "Looper must be ready");
  _looper->dispatchAsync([] { printf("Hello 3"); });
}
