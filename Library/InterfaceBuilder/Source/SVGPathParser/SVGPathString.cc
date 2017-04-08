/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <SVGPathParser/SVGPathString.h>
#include "SVGPathDriver.h"

namespace rl {
namespace svg {

geom::Path SVGPathStringParse(const std::string& pathString) {
  SVGPathDriver driver;

  if (driver.parse(pathString) != SVGPathDriver::ParserResult::Success) {
    return {};
  }

  return driver.pathBuilder().path();
}

}  // namespace svg
}  // namespace rl
