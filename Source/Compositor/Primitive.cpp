// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Compositor/Primitive.h"

using namespace rl;

Primitive::Primitive(Size size, Matrix modelViewMatrix)
    : _size(size), _modelViewMatrix(modelViewMatrix) {
}

void Primitive::setModelViewMatrix(const Matrix& modelViewMatrix) {
  if (modelViewMatrix == _modelViewMatrix) {
    return;
  }

  // Commit update
}

const Matrix& Primitive::modelViewMatrix() const {
  return _modelViewMatrix;
}

void Primitive::setSize(const Size& size) {
  if (size == _size) {
    return;
  }

  // Commit update
}

const Size& Primitive::size() const {
  return _size;
}
