// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_SVGXFORM_SVGXFORMELEMENT_H_
#define RADAR_SVGXFORM_SVGXFORMELEMENT_H_

#include <Core/Macros.h>
#include <Geometry/Matrix.h>
#include <Geometry/Rect.h>

namespace rl {

using SVGNumber = double;

SVGNumber SVGXFormNumberParse(const char* string);

class SVGXFormMatrix {
 public:
  SVGXFormMatrix() {}

  SVGXFormMatrix(SVGNumber m1,
                 SVGNumber m2,
                 SVGNumber m3,
                 SVGNumber m4,
                 SVGNumber m5,
                 SVGNumber m6) {}

  const geom::Matrix& matrix() { return _matrix; }

 private:
  geom::Matrix _matrix;
};

class SVGXFormTranslate {
 public:
  SVGXFormTranslate() {}

  SVGXFormTranslate(SVGNumber x, SVGNumber y) : _translation(x, y) {}

  const geom::Point& translation() { return _translation; }

 private:
  geom::Point _translation;
};

class SVGXFormScale {
 public:
  SVGXFormScale() {}

  SVGXFormScale(SVGNumber x, SVGNumber y) : _scale(x, y) {}

  const geom::Size& scale() { return _scale; }

 private:
  geom::Size _scale;
};

class SVGXFormRotate {
 public:
  SVGXFormRotate() {}

  SVGXFormRotate(SVGNumber radians) {}

  SVGXFormRotate(SVGNumber radians, geom::Point anchor) {}

  const SVGNumber& rotation() { return _rotation; }

 private:
  SVGNumber _rotation;
};

class SVGXFormSkewX {
 public:
  SVGXFormSkewX() {}

  SVGXFormSkewX(SVGNumber x) {}

  const SVGNumber& skew() { return _skew; }

 private:
  SVGNumber _skew;
};

class SVGXFormSkewY {
 public:
  SVGXFormSkewY() {}

  SVGXFormSkewY(SVGNumber y) {}

  const SVGNumber& skew() { return _skew; }

 private:
  SVGNumber _skew;
};

}  // namespace rl

#endif  // RADAR_SVGXFORM_SVGXFORMELEMENT_H_
