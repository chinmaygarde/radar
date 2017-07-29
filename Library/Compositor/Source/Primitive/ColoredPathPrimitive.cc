/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Compositor/Frame.h>
#include "ColoredPathPrimitive.h"
#include "ProgramCatalog.h"
#include "Uniform.h"

namespace rl {
namespace compositor {

ColoredPathPrimitive::ColoredPathPrimitive(entity::Color color,
                                           const geom::Path& path)
    : _vertices(path, FillVertices::Winding::Odd), _color(color) {}

ColoredPathPrimitive::~ColoredPathPrimitive() = default;

bool ColoredPathPrimitive::prepareToRender(BackEndPass& backEndPass) {
  return _vertices.prepare();
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
  SetUniform(program.sizeUniform(), _vertices.size());
  SetUniform(program.modelViewProjectionUniform(),
             frame.projectionMatrix() * _modelViewMatrix);

  /**
   *  Draw vertices.
   */
  return _vertices.draw(program.positionAttribute());
}

}  // namespace compositor
}  // namespace rl
