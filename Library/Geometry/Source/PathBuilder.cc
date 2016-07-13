// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Geometry/PathBuilder.h>

namespace rl {
namespace geom {

PathBuilder::PathBuilder() = default;

PathBuilder::~PathBuilder() = default;

Path PathBuilder::path() const {
  return _prototype;
}

PathBuilder& PathBuilder::moveTo(Point point) {
  _current = point;
  return *this;
}

PathBuilder& PathBuilder::lineTo(Point point) {
  _prototype.addLinearComponent(_current, point);
  _current = point;
  return *this;
}

PathBuilder& PathBuilder::quadraticCurveTo(Point point, Point controlPoint) {
  _prototype.addQuadraticComponent(_current, controlPoint, point);
  _current = point;
  return *this;
}

PathBuilder& PathBuilder::cubicCurveTo(Point point,
                                       Point controlPoint1,
                                       Point controlPoint2) {
  _prototype.addCubicComponent(_current, controlPoint1, controlPoint2, point);
  _current = point;
  return *this;
}

PathBuilder& PathBuilder::addRect(Rect rect) {
  _current = rect.origin;

  auto topLeft = rect.origin;
  auto bottomLeft = rect.origin + Point{0.0, rect.size.height};
  auto bottomRight = rect.origin + Point{rect.size.width, rect.size.height};
  auto topRight = rect.origin + Point{rect.size.width, 0.0};

  _prototype.addLinearComponent(topLeft, bottomLeft)
      .addLinearComponent(bottomLeft, bottomRight)
      .addLinearComponent(bottomRight, topRight)
      .addLinearComponent(topRight, topLeft);

  return *this;
}

PathBuilder& PathBuilder::addCircle(const Point& center, double radius) {
  _current = center + Point{0.0, radius};

  const double diameter = radius * 2.0;
  const double magic = 0.551915024494 * radius;

  _prototype.addCubicComponent({radius, 0.0},               //
                               {radius + magic, 0.0},       //
                               {diameter, radius - magic},  //
                               {diameter, radius}           //
                               );

  _prototype.addCubicComponent({diameter, radius},          //
                               {diameter, radius + magic},  //
                               {radius + magic, diameter},  //
                               {radius, diameter}           //
                               );

  _prototype.addCubicComponent({radius, diameter},          //
                               {radius - magic, diameter},  //
                               {0.0, radius + magic},       //
                               {0.0, radius}                //
                               );

  _prototype.addCubicComponent({0.0, radius},          //
                               {0.0, radius - magic},  //
                               {radius - magic, 0.0},  //
                               {radius, 0.0}           //
                               );

  return *this;
}

}  // namespace geom
}  // namespace rl
