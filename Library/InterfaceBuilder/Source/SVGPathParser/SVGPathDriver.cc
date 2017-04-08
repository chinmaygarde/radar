/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "SVGPathDriver.h"
#include "SVGPathScanner.h"

namespace rl {

SVGPathDriver::SVGPathDriver() = default;

SVGPathDriver::~SVGPathDriver() = default;

SVGPathDriver::ParserResult SVGPathDriver::parse(const std::string& text) {
  SVGPathScanner scanner(text);

  if (!scanner.isReady()) {
    return ParserResult::ParserError;
  }

  SVGPathParser parser(*this, scanner.handle());

  switch (parser.parse()) {
    case 0: /* parsing was successful (return is due to end-of-input) */
      return ParserResult::Success;
    case 1: /* contains a syntax error */
      return ParserResult::SyntaxError;
    case 2: /* memory exhaustion */
      return ParserResult::OutOfMemory;
  }

  return ParserResult::ParserError;
}

const geom::PathBuilder& SVGPathDriver::pathBuilder() const {
  return _pathBuilder;
}

void SVGPathDriver::error(rl::location loc, const std::string& message) {}

/*
 *  https://www.w3.org/TR/SVG/paths.html#PathDataMovetoCommands
 */
template <>
void SVGPathDriver::processElement(const SVGMoveElement& element) {
  _pathBuilder.moveTo(element.points().back(),
                      element.areCoordinatesRelative());
}

/*
 *  https://www.w3.org/TR/SVG/paths.html#PathDataClosePathCommand
 */
template <>
void SVGPathDriver::processElement(const SVGCloseElement& element) {
  _pathBuilder.close();
}

/*
 *  https://www.w3.org/TR/SVG/paths.html#PathDataLinetoCommands
 */
template <>
void SVGPathDriver::processElement(const SVGLineElement& element) {
  auto relative = element.areCoordinatesRelative();
  for (const auto& point : element.points()) {
    _pathBuilder.lineTo(point, relative);
  }
}

/*
 *  https://www.w3.org/TR/SVG/paths.html#PathDataLinetoCommands
 */
template <>
void SVGPathDriver::processElement(const SVGLineHorizontalElement& element) {
  auto relative = element.areCoordinatesRelative();
  for (const auto& coord : element.coords()) {
    _pathBuilder.horizontalLineTo(coord, relative);
  }
}

/*
 *  https://www.w3.org/TR/SVG/paths.html#PathDataLinetoCommands
 */
template <>
void SVGPathDriver::processElement(const SVGLineVerticalElement& element) {
  auto relative = element.areCoordinatesRelative();
  for (const auto& coord : element.coords()) {
    _pathBuilder.verticalLineTo(coord, relative);
  }
}

/*
 *  https://www.w3.org/TR/SVG/paths.html#PathDataCubicBezierCommands
 */
template <>
void SVGPathDriver::processElement(const SVGCurveElement& element) {
  auto relative = element.areCoordinatesRelative();
  size_t pointsCount = element.points().size();
  const auto& points = element.points();
  for (size_t i = 0; i < pointsCount; i += 3) {
    _pathBuilder.cubicCurveTo(points[i + 2],  // endpoint
                              points[i + 0],  // control point 1
                              points[i + 1],  // control point 2
                              relative        // relative
                              );
  }
}

/*
 *  https://www.w3.org/TR/SVG/paths.html#PathDataCubicBezierCommands
 */
template <>
void SVGPathDriver::processElement(const SVGShorthandCurveElement& element) {
  auto relative = element.areCoordinatesRelative();
  size_t pointsCount = element.points().size();
  const auto& points = element.points();
  for (size_t i = 0; i < pointsCount; i += 2) {
    _pathBuilder.smoothCubicCurveTo(points[i + 1], points[i], relative);
  }
}

/*
 *  https://www.w3.org/TR/SVG/paths.html#PathDataQuadraticBezierCommands
 */
template <>
void SVGPathDriver::processElement(const SVGQuadCurveElement& element) {
  auto relative = element.areCoordinatesRelative();
  size_t pointsCount = element.points().size();
  const auto& points = element.points();
  for (size_t i = 0; i < pointsCount; i += 2) {
    _pathBuilder.quadraticCurveTo(points[i + 1],  // endpoint
                                  points[i + 0],  // control point
                                  relative        // relative
                                  );
  }
}

/*
 *  https://www.w3.org/TR/SVG/paths.html#PathDataQuadraticBezierCommands
 */
template <>
void SVGPathDriver::processElement(
    const SVGShorthandQuadCurveElement& element) {
  auto relative = element.areCoordinatesRelative();
  size_t pointsCount = element.points().size();
  const auto& points = element.points();
  for (size_t i = 0; i < pointsCount; i++) {
    _pathBuilder.smoothQuadraticCurveTo(points[i],  // endpoint
                                        relative    // relative
                                        );
  }
}

template <>
void SVGPathDriver::processElement(const SVGEllipticArcElement& element) {
  /*
   *  TODO: WIP
   */
}

}  // namespace rl
