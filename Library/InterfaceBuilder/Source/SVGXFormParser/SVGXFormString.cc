// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

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
