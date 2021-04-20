/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "ColoredPathStrokePrimitive.h"
#include <Compositor/Frame.h>
#include "Console.h"
#include "ProgramCatalog.h"
#include "Uniform.h"

namespace rl {
namespace compositor {

ColoredPathStrokePrimitive::ColoredPathStrokePrimitive(const geom::Path& path,
                                                       entity::Color color)
    : _vertices(path), _color(color) {}

ColoredPathStrokePrimitive::~ColoredPathStrokePrimitive() = default;

bool ColoredPathStrokePrimitive::prepareToRender(BackEndPass& backEndPass) {
  return _vertices.prepare();
}

bool ColoredPathStrokePrimitive::render(Frame& frame) const {
  auto& program = frame.context().programCatalog().strokeProgram();

  if (!program.use()) {
    return false;
  }

  /*
   *  Set uniforms.
   */
  SetUniform(program.contentColorUniform(), _color, _opacity);
  SetUniform(program.sizeUniform(), _vertices.size());
  SetUniform(program.featherUniform(),
             RL_CONSOLE_GET_RANGE_ONCE("Feather", 1.0f, 0.0f, 5.0f));
  SetUniform(program.strokeSizeUniform(),
             _strokeSize + RL_CONSOLE_GET_RANGE_ONCE("Width Adjustment", 0.0f,
                                                     0.0f, 100.0f));
  SetUniform(program.totalPathDistanceUniform(),
             _vertices.totalPathDistance() /
                 RL_CONSOLE_GET_RANGE_ONCE("Dash Scale", 1.0f, 0.1f, 5.0f));
  SetUniform(program.modelViewProjectionUniform(),
             frame.projectionMatrix() * _modelViewMatrix);

  /**
   *  Draw vertices.
   */
  bool drawn =
      _vertices.draw(program.positionAttribute(), program.normalAttribute(),
                     program.segmentContinuationAttribute(),
                     program.completionDistanceAttribute());

  RL_GLAssert("No errors while rendering");

  return drawn;
}

}  // namespace compositor
}  // namespace rl
