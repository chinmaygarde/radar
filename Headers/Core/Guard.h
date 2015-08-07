// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_CORE_GUARD__
#define __RADARLOVE_CORE_GUARD__

#include <Core/Macros.h>

#include <functional>

namespace rl {
/**
 *  A guard that invokes the block when the variable goes out of scope
 */
class Guard {
 public:
  using Block = std::function<void(void)>;

  /**
   *  Create a guard with given block.
   *
   *  @param block the block to invoke when the variable is collected
   */
  Guard(const Block& block);

  ~Guard();

 private:
  const Block& _block;

  DISALLOW_COPY_AND_ASSIGN(Guard);
};
}

#endif /* defined(__RADARLOVE_CORE_GUARD__) */
