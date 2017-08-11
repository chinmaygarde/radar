/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Typography/Framesetter.h>
#include <Typography/TypographyContext.h>
#include <unicode/brkiter.h>
#include <memory>
#include <vector>

namespace rl {
namespace type {

Framesetter::Framesetter(AttributedString pString)
    : _string(std::move(pString)) {
  if (!_string.isValid()) {
    return;
  }

  auto iterator =
      TypographyContext::SharedContext().GetBreakIteratorForThread();

  if (iterator == nullptr) {
    return;
  }

  iterator->setText(_string.string().unicodeString());

  std::vector<int32_t> breakCandidates;

  for (int32_t current = iterator->first(); current != BreakIterator::DONE;
       current = iterator->next()) {
    breakCandidates.emplace_back(current);
  }

  _valid = true;
}

Framesetter::~Framesetter() = default;

bool Framesetter::isValid() const {
  return _valid;
}

}  // namespace type
}  // namespace rl
