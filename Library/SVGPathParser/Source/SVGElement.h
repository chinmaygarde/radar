// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_SVGPARSER_SVGELEMENT_H_
#define RADAR_SVGPARSER_SVGELEMENT_H_

#include <Core/Macros.h>
#include <vector>
#include <Geometry/Point.h>

namespace rl {

using SVGNumber = double;
using SVGNumbers = std::vector<SVGNumber>;
using SVGPoints = std::vector<geom::Point>;

SVGNumber SVGNumberParse(const char* string);

class SVGElement {
 public:
  SVGElement() : _valid(false), _hasAbsoluteCoordinates(true) {}

  bool isValid() const { return _valid; }

  bool areCoordinatesRelative() const { return !_hasAbsoluteCoordinates; }

 protected:
  bool _valid;
  bool _hasAbsoluteCoordinates;
};

class SVGMoveElement : public SVGElement {
 public:
  SVGMoveElement() {}

  SVGMoveElement(bool abs, SVGPoints points) : _points(std::move(points)) {
    _hasAbsoluteCoordinates = abs;
    _valid = _points.size() > 0;
  }

  const SVGPoints& points() const { return _points; }

 private:
  SVGPoints _points;
};

class SVGCloseElement : public SVGElement {
 public:
  SVGCloseElement() { _valid = true; }

 private:
};

class SVGLineElement : public SVGElement {
 public:
  SVGLineElement() {}

  SVGLineElement(bool abs, SVGPoints points) : _points(std::move(points)) {
    _hasAbsoluteCoordinates = abs;
    _valid = _points.size() > 0;
  }

  const SVGPoints& points() const { return _points; }

 private:
  SVGPoints _points;
};

class SVGLineHorizontalElement : public SVGElement {
 public:
  SVGLineHorizontalElement() {}

  SVGLineHorizontalElement(bool abs, SVGNumbers coords)
      : _coords(std::move(coords)) {
    _hasAbsoluteCoordinates = abs;
    _valid = _coords.size() > 0;
  }

  const SVGNumbers& coords() const { return _coords; }

 private:
  SVGNumbers _coords;
};

class SVGLineVerticalElement : public SVGElement {
 public:
  SVGLineVerticalElement() {}

  SVGLineVerticalElement(bool abs, SVGNumbers coords)
      : _coords(std::move(coords)) {
    _hasAbsoluteCoordinates = abs;
    _valid = _coords.size() > 0;
  }

  const SVGNumbers& coords() const { return _coords; }

 private:
  SVGNumbers _coords;
};

class SVGCurveElement : public SVGElement {
 public:
  SVGCurveElement() {}

  SVGCurveElement(bool abs, SVGPoints points) : _points(std::move(points)) {
    _hasAbsoluteCoordinates = abs;
    _valid = _points.size() > 0 && _points.size() % 3 == 0;
  }

  const SVGPoints& points() const { return _points; }

 private:
  SVGPoints _points;
};

class SVGShorthandCurveElement : public SVGElement {
 public:
  SVGShorthandCurveElement() {}

  SVGShorthandCurveElement(bool abs, SVGPoints points)
      : _points(std::move(points)) {
    _hasAbsoluteCoordinates = abs;
    _valid = _points.size() > 0 && _points.size() % 2 == 0;
  }

  const SVGPoints& points() const { return _points; }

 private:
  SVGPoints _points;
};

class SVGQuadCurveElement : public SVGElement {
 public:
  SVGQuadCurveElement() {}

  SVGQuadCurveElement(bool abs, SVGPoints points) : _points(std::move(points)) {
    _hasAbsoluteCoordinates = abs;
    _valid = _points.size() > 0 && _points.size() % 2 == 0;
  }

  const SVGPoints& points() const { return _points; }

 private:
  SVGPoints _points;
};

class SVGShorthandQuadCurveElement : public SVGElement {
 public:
  SVGShorthandQuadCurveElement() {}

  SVGShorthandQuadCurveElement(bool abs, SVGPoints points)
      : _points(std::move(points)) {
    _hasAbsoluteCoordinates = abs;
    _valid = _points.size() > 0;
  }

  const SVGPoints& points() const { return _points; }

 private:
  SVGPoints _points;
};

class SVGArcParam : public SVGElement {
 public:
  SVGArcParam() {}

  SVGArcParam(SVGNumber p1,
              SVGNumber p2,
              SVGNumber p3,
              SVGNumber p4,
              SVGNumber p5,
              SVGNumber p6,
              SVGNumber p7) {}

 private:
};

class SVGEllipticArcElement : public SVGElement {
 public:
  SVGEllipticArcElement() {}

  SVGEllipticArcElement(bool abs, const std::vector<SVGArcParam>& params) {}

 private:
};

}  // namespace rl

#endif  // RADAR_SVGPARSER_SVGELEMENT_H_
