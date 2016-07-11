// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/Primitive/ColoredPathPrimitive.h>

#include "Mesh.h"

namespace rl {
namespace compositor {

ColoredPathPrimitive::ColoredPathPrimitive(const geom::Path& path)
    : _mesh(core::make_unique<Mesh>(path,
                                    Mesh::Winding::Odd,
                                    Mesh::ElementType::Polygons)) {}

ColoredPathPrimitive::~ColoredPathPrimitive() = default;

void ColoredPathPrimitive::prepareToRender(BackEndPass& backEndPass) {}

bool ColoredPathPrimitive::render(Frame& frame) const {
  return true;
}

}  // namespace compositor
}  // namespace rl
