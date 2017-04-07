// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <Core/Macros.h>
#include <functional>

namespace rl {
namespace core {

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

  RL_DISALLOW_COPY_AND_ASSIGN(Guard);
};

}  // namespace core
}  // namespace rl
