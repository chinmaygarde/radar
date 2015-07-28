// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <gtest/gtest.h>
#include "Server.h"
#include "Looper.h"
#include <thread>

#if !__APPLE__

TEST(ServerTest, SimpleInitialization) {
  rl::Server server("/tmp/radarlove_test");
  ASSERT_TRUE(server.isListening());
}

TEST(ServerTest, AccessListenerSource) {
  rl::Server server("/tmp/radarlove_test_for_listener");
  ASSERT_TRUE(server.isListening());

  ASSERT_TRUE(server.clientConnectionsSource().get() != nullptr);
}

#endif  // __APPLE__