/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/Utilities.h>
#include <TestRunner/TestRunner.h>

TEST(UtilitiesTest, Simple) {
  ASSERT_EQ(rl::core::NextPowerOfTwoSize(0), 1u);
  ASSERT_EQ(rl::core::NextPowerOfTwoSize(1), 1u);
  ASSERT_EQ(rl::core::NextPowerOfTwoSize(2), 2u);
  ASSERT_EQ(rl::core::NextPowerOfTwoSize(3), 4u);
  ASSERT_EQ(rl::core::NextPowerOfTwoSize(14), 16u);
}
