/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <limits>

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

struct TextRange {
  size_t start = 0;
  size_t end = std::numeric_limits<size_t>::max();

  TextRange(size_t pStart, size_t pEnd) : start(pStart), end(pEnd) {}

  bool isValid() const { return end >= start; }
};

}  // namespace type
}  // namespace rl

namespace icu {

class UnicodeString;

}  // namespace icu
