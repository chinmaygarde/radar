// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Coordinator/RenderSurface.h>
#include <Core/TraceEvent.h>

namespace rl {
namespace coordinator {

RenderSurface::RenderSurface() = default;

RenderSurface::~RenderSurface() = default;

void RenderSurface::contextAccessWillBegin() {
  /*
   *  Base class does nothing.
   */
}

void RenderSurface::contextAccessDidEnd() {
  /*
   *  Base class does nothing.
   */
}

ScopedRenderSurfaceAccess::ScopedRenderSurfaceAccess(RenderSurface& surface)
    : _surface(surface), _finalized(false) {
  RL_TRACE_AUTO("SurfaceMakeCurrent");
  _acquired = _surface.makeCurrent();

  _surface.contextAccessWillBegin();
}

ScopedRenderSurfaceAccess::~ScopedRenderSurfaceAccess() {
  _surface.contextAccessDidEnd();

  if (_acquired && _finalized) {
    RL_TRACE_AUTO("SurfacePresent");
    bool result = _surface.present();
    RL_ASSERT(result);
  }
}

bool ScopedRenderSurfaceAccess::acquired() const {
  return _acquired;
}

bool ScopedRenderSurfaceAccess::finalized() const {
  return _finalized;
}

void ScopedRenderSurfaceAccess::finalizeForPresentation() {
  _finalized = true;
}

}  // namespace coordinator
}  // namespace rl
