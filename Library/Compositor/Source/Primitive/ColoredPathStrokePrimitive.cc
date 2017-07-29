/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Compositor/Frame.h>
#include "ColoredPathStrokePrimitive.h"
#include "Console.h"
#include "ProgramCatalog.h"
#include "Uniform.h"

namespace rl {
namespace compositor {

ColoredPathStrokePrimitive::ColoredPathStrokePrimitive(const geom::Path& path,
                                                       entity::Color color,
                                                       double size)
    : _vertices(path), _color(color), _strokeSize(size) {}

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

  SetUniform(program.strokeSizeUniform(),
             RL_CONSOLE_GET_RANGE_ONCE(
                 "Stroke Size", static_cast<float>(_strokeSize), 0.0, 100.0));
  SetUniform(program.modelViewProjectionUniform(),
             _modelViewMatrix * frame.projectionMatrix());

  /**
   *  Draw vertices.
   */
  bool drawn =
      _vertices.draw(program.positionAttribute(), program.normalAttribute());

  RL_GLAssert("No errors while rendering");

  return drawn;
}

}  // namespace compositor
}  // namespace rl
