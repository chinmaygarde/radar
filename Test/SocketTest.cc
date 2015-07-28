// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Socket.h"
#include <gtest/gtest.h>

#if !__APPLE__

TEST(SocketTest, SimplePairInitialization) {
  auto socketPair = rl::Socket::CreatePair();

  ASSERT_TRUE(socketPair.first.get() != nullptr);
  ASSERT_TRUE(socketPair.second.get() != nullptr);
}

#endif  // __APPLE__
