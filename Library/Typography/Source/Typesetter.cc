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

Typesetter::Typesetter(AttributedString pString)
    : _string(std::move(pString)), _runs(_string) {
  if (!_runs.isValid()) {
    return;
  }

  auto iterator = TypographyContext::SharedContext().breakIteratorForThread();

  if (iterator == nullptr) {
    return;
  }

  iterator->setText(_string.string().unicodeString());

  for (int32_t current = iterator->first(); current != BreakIterator::DONE;
       current = iterator->next()) {
    _breakOpportunities.emplace_back(current);
  }

  _valid = true;
}

Typesetter::~Typesetter() = default;

const TextRuns& Typesetter::runs() const {
  return _runs;
}

bool Typesetter::isValid() const {
  return _valid;
}

TypeFrame Typesetter::createTypeFrame(const geom::Size& size,
                                      const FontLibrary& library) const {
  if (!_valid || !size.isPositive() || !library.isValid()) {
    return {};
  }

  /*
   *  Break apart the runs at each break opportunity.
   */
  auto runs = _runs.splitAtBreaks(_breakOpportunities);

  if (!runs.isValid()) {
    return {};
  }

  return {};
}

}  // namespace type
}  // namespace rl
