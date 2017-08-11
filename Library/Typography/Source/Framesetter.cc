/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Typography/Framesetter.h>
#include <unicode/brkiter.h>
#include <memory>

namespace rl {
namespace type {

Framesetter::Framesetter(AttributedString string, geom::Size frameSize) {
  UErrorCode status = U_ZERO_ERROR;
  std::unique_ptr<BreakIterator> iterator(
      BreakIterator::createLineInstance(Locale::getDefault(), status));
  if (U_FAILURE(status) || iterator == nullptr) {
    RL_LOG("ICU Error: %s", u_errorName(status));
    return;
  }
}

Framesetter::~Framesetter() {}

bool Framesetter::isValid() const {
  return false;
}

}  // namespace type
}  // namespace rl
