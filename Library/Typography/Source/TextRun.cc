/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Typography/TextRun.h>
#include <unicode/ubidi.h>
#include <unicode/unistr.h>

namespace rl {
namespace type {

static inline TextRun::Direction ToTextRunDirection(UBiDiDirection direction) {
  switch (direction) {
    case UBIDI_LTR:
      return TextRun::Direction::LeftToRight;
    case UBIDI_RTL:
      return TextRun::Direction::RightToLeft;
    default:
      return TextRun::Direction::kUnknown;
  }
  return TextRun::Direction::kUnknown;
}

std::vector<TextRun> TextRun::SplitRuns(const AttributedString& string) {
  if (string.string().lengthOfCharacters() == 0) {
    return {};
  }

  const auto bidiClose = [](UBiDi* bidi) { ubidi_close(bidi); };
  std::unique_ptr<UBiDi, decltype(bidiClose)> bidi(ubidi_open(), bidiClose);

  if (bidi == nullptr) {
    RL_LOG("Could not create a bidi context to split text runs.");
    return {};
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
    return {};
  }

  int32_t runsCount = ubidi_countRuns(bidi.get(), &status);
  if (U_FAILURE(status)) {
    RL_LOG("Could not count bidi runs: %s", u_errorName(status));
    return {};
  }

  if (runsCount <= 0) {
    return {};
  }

  std::vector<TextRun> textRuns;
  textRuns.reserve(runsCount);

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
    if (!isRunValid) {
      continue;
    }
    textRuns.emplace_back(ToTextRunDirection(direction),
                          TextRange{static_cast<size_t>(logicalStart),
                                    static_cast<size_t>(length)});
  }

  return textRuns;
}

TextRun::TextRun(Direction direction, TextRange range)
    : _direction(direction), _range(range) {}

TextRun::~TextRun() = default;

TextRun::Direction TextRun::direction() const {
  return _direction;
}

TextRange TextRun::range() const {
  return _range;
}

}  // namespace type
}  // namespace rl
