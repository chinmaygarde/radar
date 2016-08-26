// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <TestRunner/TestRunner.h>

#include <Core/TempFileHandle.h>

TEST(TempFileTest, TempFileCreate) {
  auto handle = rl::core::TemporaryFileCreate();
  ASSERT_TRUE(handle.isValid());
}

TEST(TempFileTest, TempDirectoryGet) {
  auto uri = rl::core::TemporaryFileDirectory();
  ASSERT_TRUE(uri.isValid());
}
