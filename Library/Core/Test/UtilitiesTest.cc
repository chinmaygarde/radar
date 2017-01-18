// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Utilities.h>
#include <TestRunner/TestRunner.h>

TEST(UtilitiesTest, Simple) {
  ASSERT_EQ(rl::core::NextPowerOfTwoSize(0), 1);
  ASSERT_EQ(rl::core::NextPowerOfTwoSize(1), 1);
  ASSERT_EQ(rl::core::NextPowerOfTwoSize(2), 2);
  ASSERT_EQ(rl::core::NextPowerOfTwoSize(3), 4);
  ASSERT_EQ(rl::core::NextPowerOfTwoSize(14), 16);
}
