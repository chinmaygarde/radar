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
  if (!_string.isValid()) {
    return;
  }

  auto iterator =
      TypographyContext::SharedContext().GetBreakIteratorForThread();

  if (iterator == nullptr) {
    return;
  }

  iterator->setText(_string.string().unicodeString());

  std::vector<size_t> breakOpportunities;

  for (int32_t current = iterator->first(); current != BreakIterator::DONE;
       current = iterator->next()) {
    breakOpportunities.emplace_back(current);
  }

  _runs = TextRun::SplitRuns(_string.string(), std::move(breakOpportunities));

  _valid = true;
}

Typesetter::~Typesetter() = default;

const std::vector<TextRun> Typesetter::runs() const {
  return _runs;
}

bool Typesetter::isValid() const {
  return _valid;
}

TypeFrame Typesetter::createTypeFrame(const geom::Size& size) const {
  if (!_valid || !size.isPositive()) {
    return {};
  }

  return {};
}

}  // namespace type
}  // namespace rl
