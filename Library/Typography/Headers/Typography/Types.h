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

  bool isIndexInRange(size_t index) {
    return index >= start && index < start + length;
  }

  bool containsRange(const TextRange& other) const {
    if (length == 0 || other.length == 0) {
      return false;
    }

    return other.start >= start &&
           (other.length <= length - (other.start - start));
  }

  bool operator==(const TextRange& other) const {
    return start == other.start && length == other.length;
  }
};

}  // namespace type
}  // namespace rl

namespace icu {

class UnicodeString;
class BreakIterator;

}  // namespace icu
