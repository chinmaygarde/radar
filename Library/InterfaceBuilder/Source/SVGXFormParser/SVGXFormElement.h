/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#ifndef RADAR_SVGXFORM_SVGXFORMELEMENT_H_
#define RADAR_SVGXFORM_SVGXFORMELEMENT_H_

#include <Core/Macros.h>
#include <Geometry/Matrix.h>
#include <Geometry/Rect.h>

#define TO_RADIANS(x) ((M_PI * (x)) / 180.0)

namespace rl {

using SVGNumber = double;

SVGNumber SVGXFormNumberParse(const char* string);

class SVGXFormMatrix {
 public:
  SVGXFormMatrix() {}

  /*
   *  https://www.w3.org/TR/SVG/coords.html#TransformMatrixDefined
   */
  SVGXFormMatrix(SVGNumber a,
                 SVGNumber b,
                 SVGNumber c,
                 SVGNumber d,
                 SVGNumber e,
                 SVGNumber f)
      : _matrix(
            // clang-format off
            a,   b,   c,   0.0,
            d,   e,   f,   0.0,
            0.0, 0.0, 1.0, 0.0,
            0.0, 0.0, 0.0, 1.0
            // clang-format on
        ) {}

  geom::Matrix effectiveTransformation() const { return _matrix; }

 private:
  geom::Matrix _matrix;
};

class SVGXFormTranslate {
 public:
  SVGXFormTranslate() {}

  SVGXFormTranslate(SVGNumber x, SVGNumber y) : _translation(x, y) {}

  geom::Matrix effectiveTransformation() const {
    return geom::Matrix::Translation({_translation.x, _translation.y, 0.0});
  }

 private:
  geom::Point _translation;
};

class SVGXFormScale {
 public:
  SVGXFormScale() {}

  SVGXFormScale(SVGNumber x, SVGNumber y) : _scale(x, y) {}

  geom::Matrix effectiveTransformation() const {
    return geom::Matrix::Scale({_scale.width, _scale.height, 1.0});
  }

 private:
  geom::Size _scale;
};

class SVGXFormRotate {
 public:
  SVGXFormRotate() : _rotation(0.0) {}

  SVGXFormRotate(SVGNumber degrees) : _rotation(TO_RADIANS(degrees)) {}

  SVGXFormRotate(SVGNumber degrees, geom::Point anchor)
      : _rotation(TO_RADIANS(degrees)), _anchor(anchor) {}

  geom::Matrix effectiveTransformation() const {
    if (_anchor.x == 0.0 && _anchor.y == 0.0) {
      return geom::Matrix::RotationZ(_rotation);
    }

    return geom::Matrix::Translation(_anchor) *
           geom::Matrix::RotationZ(_rotation) *
           geom::Matrix::Translation(-_anchor);
  }

 private:
  SVGNumber _rotation;
  geom::Point _anchor;
};

class SVGXFormSkewX {
 public:
  SVGXFormSkewX() : _skew(0.0) {}

  SVGXFormSkewX(SVGNumber x) : _skew(TO_RADIANS(x)) {}

  geom::Matrix effectiveTransformation() const {
    /*
     *  https://www.w3.org/TR/SVG/coords.html#SkewXDefined
     */
    return geom::Matrix{
        1.0,          0.0, 0.0, 0.0,  //
        ::tan(_skew), 1.0, 0.0, 0.0,  //
        0.0,          0.0, 1.0, 0.0,  //
        0.0,          0.0, 0.0, 1.0,  //
    };
  }

 private:
  SVGNumber _skew;
};

class SVGXFormSkewY {
 public:
  SVGXFormSkewY() : _skew(0.0) {}

  SVGXFormSkewY(SVGNumber y) : _skew(TO_RADIANS(y)) {}

  geom::Matrix effectiveTransformation() const {
    /*
     *  https://www.w3.org/TR/SVG/coords.html#SkewYDefined
     */
    return geom::Matrix{
        1.0, ::tan(_skew), 0.0, 0.0,  //
        0.0, 1.0,          0.0, 0.0,  //
        0.0, 0.0,          1.0, 0.0,  //
        0.0, 0.0,          0.0, 1.0,  //
    };
  }

 private:
  SVGNumber _skew;
};

}  // namespace rl

#endif  // RADAR_SVGXFORM_SVGXFORMELEMENT_H_
