// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <TestRunner/TestRunner.h>

#include <Core/URI.h>

RL_DECLARE_TEST_START(FileTest)

TEST(FileTest, Simple) {
  rl::core::URI uri("file://hello/world.txt");
  ASSERT_EQ(uri.toString(), "file://hello/world.txt");
}

TEST(FileTest, FileSystemRepresentation) {
  rl::core::URI uri("file://hello/world.txt");
  ASSERT_EQ(uri.filesystemRepresentation(), "hello/world.txt");
}

RL_DECLARE_TEST_END
