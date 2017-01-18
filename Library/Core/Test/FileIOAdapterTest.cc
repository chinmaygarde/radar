// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/FileIOAdapter.h>
#include <Core/TempFileHandle.h>
#include <TestRunner/TestRunner.h>

TEST(FileIOAdapterTest, SimpleWrite) {
  rl::core::FileIOAdapter adapter(rl::core::TemporaryFileCreate(false));

  ASSERT_TRUE(adapter.seek(rl::core::FileIOAdapter::Whence::Set, 0));

  const uint8_t data[] = "Hello";
  ASSERT_EQ(adapter.write(data, sizeof(data)), sizeof(data));
}
