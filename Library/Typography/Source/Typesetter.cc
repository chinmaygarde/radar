/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Typography/TextRun.h>
#include <Typography/Typesetter.h>
#include <Typography/TypographyContext.h>
#include <unicode/brkiter.h>
#include <memory>
#include <vector>

namespace rl {
namespace type {

Typesetter::Typesetter(AttributedString pString) : _string(std::move(pString)) {
  /*
   *  Break the attributed string into runs based on content and styling.
   */
  TextRuns runs(_string);
  if (!runs.isValid()) {
    return;
  }

  /*
   *  Divide the runs along line break opportunities.
   */
  auto iterator = TypographyContext::SharedContext().breakIteratorForThread();

  if (iterator == nullptr) {
    return;
  }

  iterator->setText(_string.string().unicodeString());

  std::vector<size_t> breakOpportunities;
  for (int32_t current = iterator->first();  //
       current != BreakIterator::DONE;       //
       current = iterator->next()) {
    breakOpportunities.emplace_back(current);
  }

  auto brokenRuns = runs.splitAtBreaks(std::move(breakOpportunities));

  if (!brokenRuns.isValid()) {
    return;
  }

  _runs = std::move(brokenRuns);
  _valid = true;
}

Typesetter::~Typesetter() = default;

const TextRuns& Typesetter::runs() const {
  return _runs;
}

bool Typesetter::isValid() const {
  return _valid;
}

std::vector<ShapedTextRun> Typesetter::createShapedRuns(
    const FontLibrary& library) const {
  std::vector<ShapedTextRun> shapedRuns;
  for (const auto& run : _runs.runs()) {
    ShapedTextRun shapedRun(_string.string(), run, library);
    if (!shapedRun.isValid()) {
      RL_LOG("Could not create shaped run.");
      return {};
    }
    shapedRuns.emplace_back(std::move(shapedRun));
  }
  return shapedRuns;
}

}  // namespace type
}  // namespace rl
