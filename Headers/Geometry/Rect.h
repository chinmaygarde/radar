// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_GEOMETRY_RECT_
#define RADARLOVE_GEOMETRY_RECT_

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
  Rect operator+(const Rect& r) const {
    return Rect({origin.x + r.origin.x, origin.y + r.origin.y},
                {size.width + r.size.width, size.height + r.size.height});
  }

  Rect operator-(const Rect& r) const {
    return Rect({origin.x - r.origin.x, origin.y - r.origin.y},
                {size.width - r.size.width, size.height - r.size.height});
  }

  Rect operator*(double scale) const {
    return Rect({origin.x * scale, origin.y * scale},
                {size.width * scale, size.height * scale});
  }

  bool operator==(const Rect& r) const {
    return origin == r.origin && size == r.size;
  }
};

static const Rect RectZero(PointZero, SizeZero);
}

#endif  // RADARLOVE_GEOMETRY_RECT_
