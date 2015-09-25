// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_GEOMETRY_SIZE__
#define __RADARLOVE_GEOMETRY_SIZE__

namespace rl {
struct Size {
  double width;
  double height;

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
}

#endif  // __RADARLOVE_GEOMETRY_SIZE__
