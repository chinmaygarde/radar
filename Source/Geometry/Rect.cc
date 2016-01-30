// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Geometry/Rect.h>

#include <sstream>

namespace rl {
namespace geom {

std::string Rect::toString() const {
  std::stringstream stream;
  stream << origin.x << "," << origin.y << "," << size.width << ","
         << size.height;
  return stream.str();
}

void Rect::fromString(const std::string& str) {
  std::stringstream stream(str);
  stream >> origin.x;
  stream.ignore();
  stream >> origin.y;
  stream.ignore();
  stream >> size.width;
  stream.ignore();
  stream >> size.height;
}

}  // namespace geom
}  // namespace rl
