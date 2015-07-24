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
}
