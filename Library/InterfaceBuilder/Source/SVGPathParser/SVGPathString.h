// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_SVGPATHPARSER_SVGPATHSTRING_H_
#define RADAR_SVGPATHPARSER_SVGPATHSTRING_H_

#include <Geometry/Path.h>

namespace rl {
namespace svg {

geom::Path SVGPathStringParse(const std::string& pathString);

}  // namespace svg
}  // namespace rl

#endif  // RADAR_SVGPATHPARSER_SVGPATHSTRING_H_
