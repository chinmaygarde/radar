/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#ifndef RADAR_SVGXFORMPARSER_SVGXFORMSTRING_H_
#define RADAR_SVGXFORMPARSER_SVGXFORMSTRING_H_

#include <Core/Macros.h>
#include <Geometry/Matrix.h>
#include <string>

namespace rl {
namespace svg {

geom::Matrix SVGXFormStringParse(const std::string& xformString);

}  // namespace svg
}  // namespace rl

#endif  // RADAR_SVGXFORMPARSER_SVGXFORMSTRING_H_
