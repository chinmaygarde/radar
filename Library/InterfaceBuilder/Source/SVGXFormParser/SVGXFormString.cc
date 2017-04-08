/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "SVGXFormString.h"
#include "SVGXFormDriver.h"

namespace rl {
namespace svg {

geom::Matrix SVGXFormStringParse(const std::string& xformString) {
  if (xformString.size() == 0) {
    return {};
  }

  SVGXFormDriver driver;

  if (driver.parse(xformString) != rl::SVGXFormDriver::ParserResult::Success) {
    return {};
  }

  return driver.transformation();
}

}  // namespace svg
}  // namespace rl
