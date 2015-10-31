// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Guard.h>

namespace rl {
namespace core {

Guard::Guard(const Block& block) : _block(block) {
}

Guard::~Guard() {
  _block();
}

}  // namespace core
}  // namespace rl
