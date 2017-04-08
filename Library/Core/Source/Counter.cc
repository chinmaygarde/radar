/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/Counter.h>

namespace rl {
namespace instrumentation {

Counter::Counter() : _count(0) {}

size_t Counter::count() const {
  return _count;
}

void Counter::reset(size_t count) {
  _count = count;
}

void Counter::increment(size_t count) {
  _count += count;
}

}  // namespace instrumentation
}  // namespace rl
