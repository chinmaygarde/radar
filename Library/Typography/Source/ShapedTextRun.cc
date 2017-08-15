/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "ShapedTextRun.h"

namespace rl {
namespace type {

ShapedTextRun::ShapedTextRun() = default;

ShapedTextRun::ShapedTextRun(const String& string,
                             const TextRun& run,
                             const FontLibrary& library) {
  if (!TextRange{0, string.size()}.containsRange(run.range())) {
    return;
  }

  _font = library.fontForDescriptor(run.descriptor());

  if (!_font.isValid()) {
    return;
  }
}

ShapedTextRun::ShapedTextRun(ShapedTextRun&& o)
    : _font(std::move(o._font)), _valid(o._valid) {
  o._valid = false;
}

ShapedTextRun::~ShapedTextRun() = default;

bool ShapedTextRun::isValid() const {
  return _valid;
}

}  // namespace type
}  // namespace rl
