/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "ColoredBoxPrimitive.h"
#include <Compositor/Frame.h>
#include "ProgramCatalog.h"
#include "Uniform.h"
#include "Vertices/BoxVertices.h"

namespace rl {
namespace compositor {

ColoredBoxPrimitive::ColoredBoxPrimitive(entity::Color color) : _color(color) {}

ColoredBoxPrimitive::~ColoredBoxPrimitive() = default;

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
