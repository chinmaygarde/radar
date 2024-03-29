/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/Utilities.h>
#include <TestRunner/TestRunner.h>

namespace rl {
namespace core {
namespace testing {

TEST(UtilitiesTest, Simple) {
  ASSERT_EQ(NextPowerOfTwoSize(0), 1u);
  ASSERT_EQ(NextPowerOfTwoSize(1), 1u);
  ASSERT_EQ(NextPowerOfTwoSize(2), 2u);
  ASSERT_EQ(NextPowerOfTwoSize(3), 4u);
  ASSERT_EQ(NextPowerOfTwoSize(14), 16u);
}

}  // namespace testing
}  // namespace core
}  // namespace rl
