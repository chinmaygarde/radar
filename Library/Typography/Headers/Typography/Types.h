/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

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
