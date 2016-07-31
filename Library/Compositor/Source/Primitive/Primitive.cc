// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/Primitive/Primitive.h>

namespace rl {
namespace compositor {

const double Primitive::AlphaThreshold = 1e-3;

Primitive::Primitive() : _opacity(1.0) {}

Primitive::~Primitive() = default;

Primitive::Primitive(Primitive&& primitive) = default;

void Primitive::setSize(geom::Size size) {
  _size = size;
}

void Primitive::setModelViewMatrix(geom::Matrix modelViewMatrix) {
  _modelViewMatrix = modelViewMatrix;
}

void Primitive::setOpacity(double opacity) {
  _opacity = opacity;
}

}  // namespace compositor
}  // namespace rl
