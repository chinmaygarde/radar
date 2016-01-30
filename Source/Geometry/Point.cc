// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Geometry/Point.h>
#include <sstream>

namespace rl {
namespace geom {

std::string Point::toString() const {
  std::stringstream stream;
  stream << x << "," << y;
  return stream.str();
}

void Point::fromString(const std::string& str) {
  std::stringstream stream(str);
  stream >> x;
  stream.ignore();
  stream >> y;
}

}  // namespace geom
}  // namespace rl
