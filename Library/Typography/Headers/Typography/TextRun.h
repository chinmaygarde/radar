/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Typography/AttributedString.h>
#include <Typography/Types.h>
#include <vector>

namespace rl {
namespace type {

class TextRun {
 public:
  enum class Direction {
    Unknown,
    LeftToRight,
    RightToLeft,
  };

  static std::vector<TextRun> SplitRuns(const AttributedString& string);

  TextRun(Direction direction, TextRange range);

  ~TextRun();

  FontDescriptor descriptor() const;

  Direction direction() const;

  TextRange range() const;

 private:
  Direction _direction = Direction::Unknown;
  TextRange _range;
};

}  // namespace type
}  // namespace rl
