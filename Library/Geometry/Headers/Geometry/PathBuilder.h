// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_GEOMETRY_PATHBUILDER_H_
#define RADAR_GEOMETRY_PATHBUILDER_H_

#include <Core/Macros.h>
#include <Geometry/Path.h>
#include <Geometry/Rect.h>

namespace rl {
namespace geom {

class PathBuilder {
 public:
  PathBuilder();

  ~PathBuilder();

  Path path() const;

  PathBuilder& moveTo(Point point, bool relative = false);

  PathBuilder& close();

  PathBuilder& lineTo(Point point, bool relative = false);

  PathBuilder& horizontalLineTo(double x, bool relative = false);

  PathBuilder& verticalLineTo(double y, bool relative = false);

  PathBuilder& quadraticCurveTo(Point point,
                                Point controlPoint,
                                bool relative = false);

  PathBuilder& smoothQuadraticCurveTo(Point point, bool relative = false);

  PathBuilder& cubicCurveTo(Point point,
                            Point controlPoint1,
                            Point controlPoint2,
                            bool relative = false);

  PathBuilder& smoothCubicCurveTo(Point point,
                                  Point controlPoint2,
                                  bool relative = false);

  PathBuilder& addRect(Rect rect);

  PathBuilder& addRoundedRect(Rect rect, double radius);

  PathBuilder& addCircle(const Point& center, double radius);

  PathBuilder& addEllipse(const Point& center, const Size& size);

  struct RoundingRadii {
    double topLeft;
    double bottomLeft;
    double topRight;
    double bottomRight;

    RoundingRadii()
        : topLeft(0.0), bottomLeft(0.0), topRight(0.0), bottomRight(0.0) {}

    RoundingRadii(double pTopLeft,
                  double pBottomLeft,
                  double pTopRight,
                  double pBottomRight)
        : topLeft(pTopLeft),
          bottomLeft(pBottomLeft),
          topRight(pTopRight),
          bottomRight(pBottomRight) {}
  };

  PathBuilder& addRoundedRect(Rect rect, RoundingRadii radii);

 private:
  Point _subpathStart;
  Point _current;
  Path _prototype;

  Point reflectedQuadraticControlPoint1() const;

  Point reflectedCubicControlPoint1() const;

  RL_DISALLOW_COPY_AND_ASSIGN(PathBuilder);
};

}  // namespace geom
}  // namespace rl

#endif  // RADAR_GEOMETRY_PATHBUILDER_H_
