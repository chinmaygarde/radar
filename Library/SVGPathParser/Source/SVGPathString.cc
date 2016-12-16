// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

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
