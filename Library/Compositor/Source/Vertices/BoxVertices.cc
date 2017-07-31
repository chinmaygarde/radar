/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "Console.h"
#include "Vertices/BoxVertices.h"

namespace rl {
namespace compositor {

BoxVertices::BoxVertices(geom::Rect boxRect)
    : Vertices(Vertices::Type::Array), _boxRect(boxRect) {}

bool BoxVertices::uploadVertexData() {
  const gl::GLPoint topLeft = _boxRect.origin;
  const gl::GLPoint bottomRight = {_boxRect.origin.x + _boxRect.size.width,
                                   _boxRect.origin.y + _boxRect.size.height};

  GLfloat coords[] = {
      topLeft.x,     bottomRight.y,  // Vertex 0
      topLeft.x,     topLeft.y,      // Vertex 1
      bottomRight.x, bottomRight.y,  // Vertex 2
      bottomRight.y, topLeft.y,      // Vertex 3
  };

  glBufferData(GL_ARRAY_BUFFER, sizeof(coords), coords, GL_STATIC_DRAW);

  return true;
}

bool BoxVertices::draw(size_t positionAttributeIndex) const {
  auto bound = bindBuffer();

  if (!bound) {
    return false;
  }

  auto autoDisable = enableAttribute(positionAttributeIndex, 2, GL_FLOAT, 0, 0);

  glDrawArrays(RL_CONSOLE_GET_VALUE_ONCE("Show Box Mesh", false)
                   ? GL_LINE_LOOP
                   : GL_TRIANGLE_STRIP,
               0, 4);

  return true;
}

}  // namespace compositor
}  // namespace rl
