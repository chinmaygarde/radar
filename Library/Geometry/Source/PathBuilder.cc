// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Geometry/PathBuilder.h>

namespace rl {
namespace geom {

static const double kArcApproximationMagic = 0.551915024494;

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
  const double magic = kArcApproximationMagic * radius;

  _prototype.addCubicComponent(
      {center.x + radius, center.y},                     //
      {center.x + radius + magic, center.y},             //
      {center.x + diameter, center.y + radius - magic},  //
      {center.x + diameter, center.y + radius}           //
      );

  _prototype.addCubicComponent(
      {center.x + diameter, center.y + radius},          //
      {center.x + diameter, center.y + radius + magic},  //
      {center.x + radius + magic, center.y + diameter},  //
      {center.x + radius, center.y + diameter}           //
      );

  _prototype.addCubicComponent(
      {center.x + radius, center.y + diameter},          //
      {center.x + radius - magic, center.y + diameter},  //
      {center.x, center.y + radius + magic},             //
      {center.x, center.y + radius}                      //
      );

  _prototype.addCubicComponent({center.x, center.y + radius},          //
                               {center.x, center.y + radius - magic},  //
                               {center.x + radius - magic, center.y},  //
                               {center.x + radius, center.y}           //
                               );

  return *this;
}

PathBuilder& PathBuilder::addRoundedRect(Rect rect, double radius) {
  return radius == 0.0 ? addRect(rect)
                       : addRoundedRect(rect, {radius, radius, radius, radius});
}

PathBuilder& PathBuilder::addRoundedRect(Rect rect, RoundingRadii radii) {
  _current = rect.origin + Point{radii.topLeft, 0.0};

  const double magicTopRight = kArcApproximationMagic * radii.topRight;
  const double magicBottomRight = kArcApproximationMagic * radii.bottomRight;
  const double magicBottomLeft = kArcApproximationMagic * radii.bottomLeft;
  const double magicTopLeft = kArcApproximationMagic * radii.topLeft;

  /*
   *  Top line.
   */
  _prototype.addLinearComponent(
      {rect.origin.x + radii.topLeft, rect.origin.y},
      {rect.origin.x + rect.size.width - radii.topRight, rect.origin.y});

  /*
   *  Top right arc.
   */
  _prototype.addCubicComponent(
      {rect.origin.x + rect.size.width - radii.topRight, rect.origin.y},
      {rect.origin.x + rect.size.width - radii.topRight + magicTopRight,
       rect.origin.y},
      {rect.origin.x + rect.size.width,
       rect.origin.y + radii.topRight - magicTopRight},
      {rect.origin.x + rect.size.width, rect.origin.y + radii.topRight});

  /*
   *  Right line.
   */
  _prototype.addLinearComponent(
      {rect.origin.x + rect.size.width, rect.origin.y + radii.topRight},
      {rect.origin.x + rect.size.width,
       rect.origin.y + rect.size.height - radii.bottomRight});

  /*
   *  Bottom right arc.
   */
  _prototype.addCubicComponent(
      {rect.origin.x + rect.size.width,
       rect.origin.y + rect.size.height - radii.bottomRight},
      {rect.origin.x + rect.size.width,
       rect.origin.y + rect.size.height - radii.bottomRight + magicBottomRight},
      {rect.origin.x + rect.size.width - radii.bottomRight + magicBottomRight,
       rect.origin.y + rect.size.height},
      {rect.origin.x + rect.size.width - radii.bottomRight,
       rect.origin.y + rect.size.height});

  /*
   *  Bottom line.
   */
  _prototype.addLinearComponent(
      {rect.origin.x + rect.size.width - radii.bottomRight,
       rect.origin.y + rect.size.height},
      {rect.origin.x + radii.bottomLeft, rect.origin.y + rect.size.height});

  /*
   *  Bottom left arc.
   */
  _prototype.addCubicComponent(
      {rect.origin.x + radii.bottomLeft, rect.origin.y + rect.size.height},
      {rect.origin.x + radii.bottomLeft - magicBottomLeft,
       rect.origin.y + rect.size.height},
      {rect.origin.x,
       rect.origin.y + rect.size.height - radii.bottomLeft + magicBottomLeft},
      {rect.origin.x, rect.origin.y + rect.size.height - radii.bottomLeft});

  /*
   *  Left line.
   */
  _prototype.addLinearComponent(
      {rect.origin.x, rect.origin.y + rect.size.height - radii.bottomLeft},
      {rect.origin.x, rect.origin.y + radii.topLeft});

  /*
   *  Top left arc.
   */
  _prototype.addCubicComponent(
      {rect.origin.x, rect.origin.y + radii.topLeft},
      {rect.origin.x, rect.origin.y + radii.topLeft - magicTopLeft},
      {rect.origin.x + radii.topLeft - magicTopLeft, rect.origin.y},
      {rect.origin.x + radii.topLeft, rect.origin.y});

  return *this;
}

}  // namespace geom
}  // namespace rl
