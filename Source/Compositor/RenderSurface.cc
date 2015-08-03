// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Compositor/RenderSurface.h"
#include <cassert>

namespace rl {

void RenderSurface::surfaceWasCreated() {
  if (_observer != nullptr) {
    _observer->surfaceWasCreated();
  }
}

void RenderSurface::surfaceSizeUpdated(double width, double height) {
  if (_observer != nullptr) {
    _observer->surfaceSizeUpdated(width, height);
  }
}

void RenderSurface::surfaceWasDestroyed() {
  if (_observer != nullptr) {
    _observer->surfaceWasDestroyed();
  }
}

void RenderSurface::setObserver(RenderSurfaceObserver* observer) {
  _observer = observer;
}

}  // namespace rl
