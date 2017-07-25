/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "Primitive/Primitive.h"

namespace rl {
namespace compositor {

const double Primitive::AlphaThreshold = 1e-3;

const double Primitive::StrokeThreshold = 1e-3;

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
