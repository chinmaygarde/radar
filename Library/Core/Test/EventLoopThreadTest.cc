// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <TestRunner/TestRunner.h>

#include <Core/EventLoopThread.h>

TEST(NamespaceTest, SimpleInitShutdown) {
  rl::core::EventLoopThread thread;
  ASSERT_TRUE(thread.loop().get() != nullptr);
}
