// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/Primitive/ColoredBoxPrimitive.h>
#include <Compositor/Frame.h>

#include "Uniform.h"
#include "ProgramCatalog.h"
#include "Vertices/BoxVertices.h"

namespace rl {
namespace compositor {

ColoredBoxPrimitive::ColoredBoxPrimitive() = default;

ColoredBoxPrimitive::~ColoredBoxPrimitive() = default;

void ColoredBoxPrimitive::setColor(entity::Color color) {
  _color = std::move(color);
}

void ColoredBoxPrimitive::prepareToRender(BackEndPass& backEndPass) {}

bool ColoredBoxPrimitive::render(Frame& frame) const {
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
  SetUniform(program.sizeUniform(), _size);
  SetUniform(program.modelViewProjectionUniform(),
             frame.projectionMatrix() * _modelViewMatrix);

  /**
   *  Draw vertices.
   */
  bool drawn =
      frame.context().unitBoxVertices().draw(program.positionAttribute());

  RL_GLAssert("No errors while rendering");

  return drawn;
}

}  // namespace compositor
}  // namespace rl
