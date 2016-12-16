// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "SVGPathDriver.h"
#include "SVGPathScanner.h"

namespace rl {

SVGPathDriver::SVGPathDriver() {}

SVGPathDriver::~SVGPathDriver() {}

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

void SVGPathDriver::error(rl::location loc, const std::string& message) {
  printf("Error: %s\n", message.c_str());
}

template <>
void SVGPathDriver::processElement(const SVGMoveElement& element) {
  _pathBuilder.moveTo(element.points().back(),
                      element.areCoordinatesRelative());
}

template <>
void SVGPathDriver::processElement(const SVGCloseElement& element) {
  _pathBuilder.close();
}

template <>
void SVGPathDriver::processElement(const SVGLineElement& element) {
  auto relative = element.areCoordinatesRelative();
  for (const auto& point : element.points()) {
    _pathBuilder.lineTo(point, relative);
  }
}

template <>
void SVGPathDriver::processElement(const SVGLineHorizontalElement& element) {
  auto relative = element.areCoordinatesRelative();
  for (const auto& coord : element.coords()) {
    _pathBuilder.horizontalLineTo(coord, relative);
  }
}

template <>
void SVGPathDriver::processElement(const SVGLineVerticalElement& element) {
  auto relative = element.areCoordinatesRelative();
  for (const auto& coord : element.coords()) {
    _pathBuilder.verticalLineTo(coord, relative);
  }
}

template <>
void SVGPathDriver::processElement(const SVGCurveElement& element) {}

template <>
void SVGPathDriver::processElement(const SVGShorthandCurveElement& element) {}

template <>
void SVGPathDriver::processElement(const SVGQuadCurveElement& element) {}

template <>
void SVGPathDriver::processElement(
    const SVGShorthandQuadCurveElement& element) {}

template <>
void SVGPathDriver::processElement(const SVGEllipticArcElement& element) {}

template <>
void SVGPathDriver::processElement(const SVGArcParam& element) {}

}  // namespace rl
