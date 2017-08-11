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
  size_t length = 0;

  TextRange() {}

  TextRange(size_t pStart, size_t pLength) : start(pStart), length(pLength) {}
};

}  // namespace type
}  // namespace rl

namespace icu {

class UnicodeString;
class BreakIterator;

}  // namespace icu
