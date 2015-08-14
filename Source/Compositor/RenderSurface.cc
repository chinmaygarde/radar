// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/RenderSurface.h>
#include <cassert>

namespace rl {

void RenderSurface::surfaceWasCreated() {
  if (_observer != nullptr) {
    _observer->surfaceWasCreated();
  }
}

void RenderSurface::surfaceSizeUpdated(const Size& size) {
  if (_observer != nullptr) {
    _observer->surfaceSizeUpdated(size);
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

ScopedRenderSurfaceAccess::ScopedRenderSurfaceAccess(RenderSurface& surface)
    : _surface(surface) {
  bool result = _surface.makeCurrent();
  assert(result);
}

ScopedRenderSurfaceAccess::~ScopedRenderSurfaceAccess() {
  bool result = _surface.present();
  assert(result);
}

}  // namespace rl
