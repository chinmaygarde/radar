/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>

namespace rl {
namespace instrumentation {

class Counter {
 public:
  Counter();

  size_t count() const;

  void reset(size_t count = 0);

  void increment(size_t count = 1);

 private:
  size_t _count;

  RL_DISALLOW_COPY_AND_ASSIGN(Counter);
};

}  // namespace instrumentation
}  // namespace rl
