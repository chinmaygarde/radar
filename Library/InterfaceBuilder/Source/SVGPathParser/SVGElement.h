/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#ifndef RADAR_SVGPARSER_SVGELEMENT_H_
#define RADAR_SVGPARSER_SVGELEMENT_H_

#include <Core/Macros.h>
#include <Geometry/Point.h>
#include <vector>

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

class SVGArcParam {
 public:
  SVGArcParam() {}

  SVGArcParam(SVGNumber rx,
              SVGNumber ry,
              SVGNumber xRotation,
              SVGNumber largeArc,
              SVGNumber sweep,
              SVGNumber endX,
              SVGNumber endY)
      : _radii(rx, ry),
        _endPoint(endX, endY),
        _xRotation(xRotation),
        _largeArc(largeArc != 0),
        _sweep(sweep != 0) {}

  const geom::Point& radii() const { return _radii; }

  const geom::Point& endPoint() const { return _endPoint; }

  SVGNumber xRotation() const { return _xRotation; }

  bool largeArc() const { return _largeArc; }

  bool sweep() const { return _sweep; }

 private:
  geom::Point _radii;
  geom::Point _endPoint;
  SVGNumber _xRotation;
  bool _largeArc;
  bool _sweep;
};

class SVGEllipticArcElement : public SVGElement {
 public:
  SVGEllipticArcElement() {}

  SVGEllipticArcElement(bool abs, std::vector<SVGArcParam> params)
      : _params(std::move(params)) {
    _hasAbsoluteCoordinates = abs;
    _valid = _params.size() > 0;
  }

  const std::vector<SVGArcParam>& arcParams() const { return _params; }

 private:
  std::vector<SVGArcParam> _params;
};

}  // namespace rl

#endif  // RADAR_SVGPARSER_SVGELEMENT_H_
