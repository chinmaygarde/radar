/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#ifndef RADAR_SVGPATHPARSER_SVGPATHSTRING_H_
#define RADAR_SVGPATHPARSER_SVGPATHSTRING_H_

#include <Geometry/Path.h>

namespace rl {
namespace svg {

geom::Path SVGPathStringParse(const std::string& pathString);

}  // namespace svg
}  // namespace rl

#endif  // RADAR_SVGPATHPARSER_SVGPATHSTRING_H_
