// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Vertices/BoxVertices.h"

namespace rl {
namespace compositor {

BoxVertices::BoxVertices(geom::Rect boxRect)
    : Vertices(Vertices::Type::Array), _boxRect(boxRect) {}

bool BoxVertices::uploadVertexData() {
  const GLPoint topLeft = _boxRect.origin;
  const GLPoint bottomRight = {_boxRect.origin.x + _boxRect.size.width,
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

bool BoxVertices::doDraw(size_t index) {
  RL_GLAssert("There must be no errors before drawing box vertices");

  glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

  glEnableVertexAttribArray(index);

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glDisableVertexAttribArray(index);

  return true;
}

}  // namespace compositor
}  // namespace rl
