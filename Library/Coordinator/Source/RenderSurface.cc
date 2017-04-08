/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Coordinator/RenderSurface.h>
#include <Core/TraceEvent.h>

namespace rl {
namespace coordinator {

RenderSurface::RenderSurface() = default;

RenderSurface::~RenderSurface() = default;

ScopedRenderSurfaceAccess::ScopedRenderSurfaceAccess(RenderSurface& surface)
    : _surface(surface), _finalized(false) {
  RL_TRACE_AUTO("SurfaceMakeCurrent");
  _surface.accessWillBegin();

  _acquired = _surface.makeCurrent();
}

ScopedRenderSurfaceAccess::~ScopedRenderSurfaceAccess() {
  if (_acquired && _finalized) {
    RL_TRACE_AUTO("SurfacePresent");
    bool result = _surface.present();
    RL_ASSERT(result);
  }

  _surface.accessDidEnd();
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
