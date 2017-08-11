/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Typography/TypeFrame.h>
#include <hb.h>

namespace rl {
namespace type {

TypeFrame::TypeFrame() = default;

TypeFrame::TypeFrame(TypeFrame&& o) : _valid(o._valid) {
  o._valid = false;
}

TypeFrame::~TypeFrame() = default;

bool TypeFrame::isValid() const {
  return _valid;
}

}  // namespace type
}  // namespace rl
