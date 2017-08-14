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

  TextRun(Direction direction, TextRange range);

  ~TextRun();

  FontDescriptor descriptor() const;

  Direction direction() const;

  TextRange range() const;

  void setRange(TextRange range);

 private:
  Direction _direction = Direction::Unknown;
  TextRange _range;
};

class TextRuns {
 public:
  TextRuns(const AttributedString& string);

  /*
   *  For Tests Only.
   */
  TextRuns(std::vector<TextRun> runs);

  TextRuns(TextRuns&&);

  ~TextRuns();

  bool isValid() const;

  const std::vector<TextRun>& runs() const;

  RL_WARN_UNUSED_RESULT
  TextRuns splitAtBreaks(const std::vector<size_t>& breaks) const;

  size_t totalLength() const;

 private:
  std::vector<TextRun> _runs;
  bool _valid = false;

  TextRuns();

  RL_DISALLOW_COPY_AND_ASSIGN(TextRuns);
};

}  // namespace type
}  // namespace rl
