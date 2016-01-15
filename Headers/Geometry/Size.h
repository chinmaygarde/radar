// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_GEOMETRY_SIZE_
#define RADARLOVE_GEOMETRY_SIZE_

namespace rl {
namespace geom {

struct Size {
  double width;
  double height;

  Size() : width(0.0), height(0.0) {}

  Size(double width, double height) : width(width), height(height) {}

  /*
   *  Operator overloads
   */
  Size operator*(double scale) const {
    return Size(width * scale, height * scale);
  }

  bool operator==(const Size& s) const {
    return s.width == width && s.height == height;
  }

  bool operator!=(const Size& s) const {
    return s.width != width || s.height != height;
  }

  Size operator+(const Size& s) const {
    return Size(width + s.width, height + s.height);
  }

  Size operator-(const Size& s) const {
    return Size(width - s.width, height - s.height);
  }
};

static const Size SizeZero(0.0, 0.0);

}  // namespace geom
}  // namespace rl

#endif  // RADARLOVE_GEOMETRY_SIZE_
