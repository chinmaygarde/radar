// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

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
