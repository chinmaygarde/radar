/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Typography/TextRun.h>
#include <unicode/ubidi.h>
#include <unicode/unistr.h>
#include <algorithm>

namespace rl {
namespace type {

static inline TextRun::Direction ToTextRunDirection(UBiDiDirection direction) {
  switch (direction) {
    case UBIDI_LTR:
      return TextRun::Direction::LeftToRight;
    case UBIDI_RTL:
      return TextRun::Direction::RightToLeft;
    default:
      return TextRun::Direction::Unknown;
  }
  return TextRun::Direction::Unknown;
}

TextRun::TextRun(FontDescriptor descriptor,
                 Direction direction,
                 TextRange range)
    : _descriptor(std::move(descriptor)),
      _direction(direction),
      _range(range) {}

TextRun::~TextRun() = default;

FontDescriptor TextRun::descriptor() const {
  return _descriptor;
}

TextRun::Direction TextRun::direction() const {
  return _direction;
}

TextRange TextRun::range() const {
  return _range;
}

void TextRun::setRange(rl::type::TextRange range) {
  _range = std::move(range);
}

/*
 *  ----------------------------------------------------------------------------
 *  Text Runs
 *  ----------------------------------------------------------------------------
 */

TextRuns::TextRuns(const AttributedString& string) {
  if (string.string().lengthOfCharacters() == 0) {
    return;
  }

  const auto bidiClose = [](UBiDi* bidi) { ubidi_close(bidi); };
  std::unique_ptr<UBiDi, decltype(bidiClose)> bidi(ubidi_open(), bidiClose);

  if (bidi == nullptr) {
    RL_LOG("Could not create a bidi context to split text runs.");
    return;
  }

  const UnicodeString& unicodeString = string.string().unicodeString();

  UErrorCode status = U_ZERO_ERROR;
  ubidi_setPara(bidi.get(),                 // bidi
                unicodeString.getBuffer(),  // text
                unicodeString.length(),     // length
                UBIDI_DEFAULT_LTR,          // paragraph level
                nullptr,                    // embedding levels
                &status                     // error code
  );
  if (U_FAILURE(status)) {
    RL_LOG("Could not set the paragraph to split text runs: %s",
           u_errorName(status));
    return;
  }

  int32_t runsCount = ubidi_countRuns(bidi.get(), &status);
  if (U_FAILURE(status)) {
    RL_LOG("Could not count bidi runs: %s", u_errorName(status));
    return;
  }

  if (runsCount <= 0) {
    return;
  }

  _runs.reserve(runsCount);

  for (int32_t i = 0; i < runsCount; i++) {
    int32_t logicalStart = 0;
    int32_t length = 0;
    const UBiDiDirection direction =
        ubidi_getVisualRun(bidi.get(),     // bidi
                           i,              // run index
                           &logicalStart,  // logical start (out)
                           &length         // length (out)
        );

    if (logicalStart < 0 || length < 0) {
      continue;
    }
    const bool isRunValid =
        direction != UBIDI_MIXED && direction != UBIDI_NEUTRAL;
    RL_ASSERT_MSG(isRunValid, "Encountered mixed or unknown run.");
    _runs.emplace_back(
        string.descriptorForIndex(static_cast<size_t>(logicalStart)),
        ToTextRunDirection(direction),
        TextRange{static_cast<size_t>(logicalStart),
                  static_cast<size_t>(length)});
  }

  RL_ASSERT(totalLength() == string.string().size());

  _valid = true;
}

TextRuns::TextRuns() = default;

TextRuns::TextRuns(std::vector<TextRun> runs)
    : _runs(std::move(runs)), _valid(true) {}

TextRuns::TextRuns(TextRuns&& o) {
  _runs = std::move(o._runs);
  _valid = o._valid;
  o._valid = false;
}

TextRuns::~TextRuns() = default;

bool TextRuns::isValid() const {
  return _valid;
}

const std::vector<TextRun>& TextRuns::runs() const {
  return _runs;
}

TextRuns TextRuns::splitAtBreaks(const std::vector<size_t>& breaks) const {
  RL_ASSERT(std::is_sorted(breaks.begin(), breaks.end()));

  if (_runs.size() == 0) {
    /*
     *  If there are no runs, there is nothing to break.
     */
    return {};
  }

  if (breaks.size() == 0) {
    /*
     *  If there are no breaks, the current runs are valid.
     */
    return {runs()};
  }

  std::vector<TextRun> splitRuns;
  size_t breakIndex = 0;
  for (const auto& run : _runs) {
    if (!run.range().isIndexInRange(breaks[breakIndex])) {
      /*
       *  There was no break in this run, add it as-is.
       */
      if (run.range().length != 0) {
        splitRuns.emplace_back(run);
      }
      continue;
    }

    /*
     *  This run needs to be split at the breaks. n + 1 where n is the number of
     *  breaks that lie within this run.
     */
    size_t lastStart = run.range().start;
    for (; breakIndex < breaks.size(); breakIndex++) {
      size_t currentBreak = breaks[breakIndex];
      if (run.range().isIndexInRange(currentBreak)) {
        /*
         *  Keep looping till we find break in this range. These are the n
         *  entries described in the comment above.
         */
        TextRange range(lastStart, currentBreak - lastStart);
        lastStart += range.length;
        if (range.length != 0) {
          TextRun splitRun = run;
          splitRun.setRange(range);
          splitRuns.emplace_back(std::move(splitRun));
        }
      } else {
        /*
         *  Break out of the loop and look at the next run.
         */
        break;
      }
    }
    /*
     *  Add the final entry and we are done with this run. This is the + 1
     *  described in the comment above.
     */
    TextRange range(lastStart,
                    run.range().start + run.range().length - lastStart);
    if (range.length != 0) {
      TextRun splitRun = run;
      splitRun.setRange(range);
      splitRuns.emplace_back(std::move(splitRun));
    }
  }

  return {std::move(splitRuns)};
}

size_t TextRuns::totalLength() const {
  size_t length = 0;
  for (const auto& run : _runs) {
    length += run.range().length;
  }
  return length;
}

}  // namespace type
}  // namespace rl
