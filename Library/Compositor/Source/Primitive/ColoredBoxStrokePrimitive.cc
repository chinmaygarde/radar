/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Compositor/Context.h>
#include <Compositor/Frame.h>
#include "ColoredBoxStrokePrimitive.h"
#include "Console.h"
#include "ProgramCatalog.h"
#include "Uniform.h"
#include "Vertices/StrokeVertices.h"

namespace rl {
namespace compositor {

ColoredBoxStrokePrimitive::ColoredBoxStrokePrimitive(entity::Color color)
    : _color(color) {}

ColoredBoxStrokePrimitive::~ColoredBoxStrokePrimitive() = default;

bool ColoredBoxStrokePrimitive::prepareToRender(BackEndPass& backEndPass) {
  /*
   *  Nothing to render since we are going to use the frame context for prepared
   *  set of vertices.
   */
  return true;
}

bool ColoredBoxStrokePrimitive::render(Frame& frame) const {
  auto& program = frame.context().programCatalog().strokeProgram();

  if (!program.use()) {
    return false;
  }

  auto& vertices = frame.context().unitBoxStrokeVertices();

  /*
   *  Set uniforms.
   */
  SetUniform(program.contentColorUniform(), _color, _opacity);
  SetUniform(program.sizeUniform(), _size);
  SetUniform(program.featherUniform(),
             RL_CONSOLE_GET_RANGE_ONCE("Box Stroke Feather",
                                       static_cast<float>(1.0), 0.0, 5.0));
  SetUniform(program.strokeSizeUniform(), _strokeSize);
  SetUniform(program.modelViewProjectionUniform(),
             frame.projectionMatrix() * _modelViewMatrix);

  /**
   *  Draw vertices.
   */
  bool drawn =
      vertices.draw(program.positionAttribute(), program.normalAttribute(),
                    program.segmentContinuationAttribute());

  RL_GLAssert("No errors while rendering");

  return drawn;
}

}  // namespace compositor
}  // namespace rl
