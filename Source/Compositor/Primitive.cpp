// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Compositor/Primitive.h"

#define RL_OPENGL_ALLOWED 1
#include "Compositor/OpenGL.h"

using namespace rl;

Primitive::Primitive()
    : _size(SizeZero),
      _modelViewMatrix(MatrixIdentity),
      _contentColor(ColorWhiteTransparent),
      _verticesDirty(true) {
}

void Primitive::setModelViewMatrix(const Matrix& modelViewMatrix) {
  if (modelViewMatrix == _modelViewMatrix) {
    return;
  }

  _verticesDirty = true;
}

const Matrix& Primitive::modelViewMatrix() const {
  return _modelViewMatrix;
}

void Primitive::setSize(const Size& size) {
  if (size == _size) {
    return;
  }

  _verticesDirty = true;
}

const Size& Primitive::size() const {
  return _size;
}

void Primitive::setSizeAndModelViewMatrix(const Size& size,
                                          const Matrix& modelViewMatrix) {
  if (_size != size) {
    _size = size;
    _verticesDirty = true;
  }

  if (_modelViewMatrix != modelViewMatrix) {
    _modelViewMatrix = modelViewMatrix;
    _verticesDirty = true;
  }
}

const Color& Primitive::contentColor() const {
  return _contentColor;
}

void Primitive::setContentColor(const Color& color) {
  if (_contentColor == color) {
    return;
  }

  _contentColor = color;
}

void Primitive::updateVerticesIfNecessary() {
  if (!_verticesDirty) {
    return;
  }

  _verticesDirty = false;

  // Update Vertices
}

void Primitive::render(Frame& frame) {
  updateVerticesIfNecessary();

  /*
   *  Select the program to use
   */
  auto program = static_cast<const BasicPrimitiveProgram*>(
      frame.programCatalog()->useProgramType(
          ProgramCatalog::Type::BasicPrimitve));

  // clang-format off
  GLCoord coords[] = {
    { -0.5,  0.5, 0.0 },
    { -0.5, -0.5, 0.0 },
    {  0.5,  0.5, 0.0 },
    {  0.5, -0.5, 0.0 },
  };
  // clang-format on

  /**
   *  Upload Vertex Data.
   */
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, coords);

  /**
   *  Setup Uniform Data
   */
  static double foo = 0.0;
  foo += 0.01;
  auto rotation = Matrix::RotationZ(foo);

  GLMatrix model = rotation * MatrixIdentity.scale({_size.width, _size.height});
  glUniformMatrix4fv(program->modelUniform(), 1, GL_FALSE,
                     reinterpret_cast<const GLfloat*>(&model));

  GLMatrix view = MatrixIdentity;
  glUniformMatrix4fv(program->viewUniform(), 1, GL_FALSE,
                     reinterpret_cast<const GLfloat*>(&view));

  GLMatrix projection = frame.projectionMatrix();
  glUniformMatrix4fv(program->projectionUniform(), 1, GL_FALSE,
                     reinterpret_cast<const GLfloat*>(&projection));

  glUniform4f(program->contentColorUniform(), _contentColor.r, _contentColor.g,
              _contentColor.b, _contentColor.a);

  glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(coords) / sizeof(GLCoord));

  RL_GLAssert("No errors while rendering");
}
