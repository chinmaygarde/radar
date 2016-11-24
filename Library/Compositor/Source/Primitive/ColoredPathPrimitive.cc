// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ColoredPathPrimitive.h"

#include <Compositor/Frame.h>

#include "Uniform.h"
#include "ProgramCatalog.h"
#include "Vertices/PathVertices.h"

namespace rl {
namespace compositor {

ColoredPathPrimitive::ColoredPathPrimitive(entity::Color color,
                                           const geom::Path& path)
    : _pathVertices(
          core::make_unique<PathVertices>(path,
                                          PathVertices::Winding::Odd,
                                          PathVertices::ElementType::Polygons)),
      _color(color) {}

ColoredPathPrimitive::~ColoredPathPrimitive() = default;

void ColoredPathPrimitive::prepareToRender(BackEndPass& backEndPass) {
  /*
   *  Nothing to do.
   */
}

bool ColoredPathPrimitive::render(Frame& frame) const {
  /*
   *  Select the program to use
   */
  auto& program = frame.context().programCatalog().colorProgram();

  if (!program.use()) {
    return false;
  }

  /*
   *  Set uniforms.
   */
  SetUniform(program.contentColorUniform(), _color, _opacity);
  SetUniform(program.sizeUniform(), _pathVertices->size());
  SetUniform(program.modelViewProjectionUniform(),
             frame.projectionMatrix() * _modelViewMatrix);

  /**
   *  Draw vertices.
   */
  return _pathVertices->draw(program.positionAttribute());
}

}  // namespace compositor
}  // namespace rl
