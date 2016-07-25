// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_TYPOGRAPHY_TYPES_H_
#define RADAR_TYPOGRAPHY_TYPES_H_

namespace rl {
namespace type {

using Point = double;

using Codepoint = unsigned int;

struct Coordinate {
  Point x;
  Point y;

  Coordinate() : x(0.0), y(0.0) {}

  Coordinate(Point aX, Point aY) : x(aX), y(aY) {}
};

}  // namespace type
}  // namespace rl

#endif  // RADAR_TYPOGRAPHY_TYPES_H_
