/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "NullRenderSurface.h"

namespace rl {
namespace shell {

NullRenderSurface::NullRenderSurface() = default;

bool NullRenderSurface::makeCurrent() {
  return false;
}

bool NullRenderSurface::present() {
  return false;
}

void NullRenderSurface::accessWillBegin() {}

void NullRenderSurface::accessDidEnd() {}

}  // namespace shell
}  // namespace rl
