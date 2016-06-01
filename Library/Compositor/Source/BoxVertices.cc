// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "BoxVertices.h"

namespace rl {
namespace compositor {

BoxVertices::BoxVertices(geom::Rect boxRect) : _boxRect(boxRect) {}

bool BoxVertices::use() {
  glBindBuffer(GL_ARRAY_BUFFER, vbo());
  return true;
}

bool BoxVertices::uploadVertexData() {
  if (!use()) {
    return false;
  }

  // clang-format off
  GLfloat coords[] = {
    0.0, 1.0, 0.0,
    0.0, 0.0, 0.0,
    1.0, 1.0, 0.0,
    1.0, 0.0, 0.0,
  };
  // clang-format on

  glBufferData(GL_ARRAY_BUFFER, sizeof(coords), coords, GL_STATIC_DRAW);
  return true;
}

bool BoxVertices::draw() {
  if (!prepare()) {
    return false;
  }

  if (!use()) {
    return false;
  }

  RL_GLAssert("There must be no errors before drawing box vertices");

  /*
   *  Index 0 contains three floats per vertex.
   */
  static const GLuint kVertexIndex = 0;

  glVertexAttribPointer(kVertexIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(kVertexIndex);

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  return true;
}

}  // namespace compositor
}  // namespace rl
