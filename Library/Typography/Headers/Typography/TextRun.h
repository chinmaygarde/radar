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
    kUnknown,
    LeftToRight,
    RightToLeft,
  };

  static std::vector<TextRun> SplitRuns(const String& string,
                                        std::vector<size_t> breakOpportunities);

  TextRun(Direction direction, TextRange range);

  ~TextRun();

  Direction direction() const;

  TextRange range() const;

 private:
  Direction _direction = Direction::kUnknown;
  TextRange _range;
};

}  // namespace type
}  // namespace rl
