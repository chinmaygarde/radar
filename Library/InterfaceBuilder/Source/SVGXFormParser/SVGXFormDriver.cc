/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "SVGXFormDriver.h"
#include "SVGXFormScanner.h"

namespace rl {

SVGXFormDriver::SVGXFormDriver() = default;

SVGXFormDriver::~SVGXFormDriver() = default;

SVGXFormDriver::ParserResult SVGXFormDriver::parse(const std::string& text) {
  SVGXFormScanner scanner(text);

  if (!scanner.isReady()) {
    return ParserResult::ParserError;
  }

  SVGXFormParser parser(*this, scanner.handle());

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

void SVGXFormDriver::error(rl::location loc, const std::string& message) {}

const geom::Matrix& SVGXFormDriver::transformation() const {
  return _transformation;
}

}  // namespace rl
