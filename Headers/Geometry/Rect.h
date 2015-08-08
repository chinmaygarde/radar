// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_GEOMETRY_RECT__
#define __RADARLOVE_GEOMETRY_RECT__

#include <Geometry/Point.h>
#include <Geometry/Size.h>

namespace rl {
struct Rect {
  Point origin;
  Size size;

  Rect(Point origin, Size size) : origin(origin), size(size) {}
  Rect(double x, double y, double width, double height)
      : origin(x, y), size(width, height) {}

  /*
   *  Operator overloads
   */
  bool operator==(const Rect& r) const {
    return origin == r.origin && size == r.size;
  }
};

static const Rect RectZero(PointZero, SizeZero);
}

#endif /* defined(__RADARLOVE_GEOMETRY_RECT__) */
