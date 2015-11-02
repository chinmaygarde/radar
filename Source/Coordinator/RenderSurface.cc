// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Coordinator/RenderSurface.h>

namespace rl {
namespace coordinator {

void RenderSurface::surfaceWasCreated() {
  if (_observer != nullptr) {
    _observer->surfaceWasCreated();
  }
}

void RenderSurface::surfaceSizeUpdated(const geom::Size& size) {
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
  RL_ASSERT(result);
}

ScopedRenderSurfaceAccess::~ScopedRenderSurfaceAccess() {
  bool result = _surface.present();
  RL_ASSERT(result);
}

}  // namespace coordinator
}  // namespace rl