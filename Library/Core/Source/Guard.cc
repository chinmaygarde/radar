/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/Guard.h>

namespace rl {
namespace core {

Guard::Guard(const Block& block) : _block(block) {}

Guard::~Guard() {
  _block();
}

}  // namespace core
}  // namespace rl
