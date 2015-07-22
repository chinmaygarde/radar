// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_CORE_GUARD__
#define __RADARLOVE_CORE_GUARD__

#include "Core/Base.h"

#include <functional>

namespace rl {
class Guard {
 public:
  using Block = std::function<void(void)>;

  Guard(const Block& block) : _block(block) {}
  ~Guard() { _block(); };

 private:
  const Block& _block;
  DISALLOW_COPY_AND_ASSIGN(Guard);
};
}

#endif /* defined(__RADARLOVE_CORE_GUARD__) */
