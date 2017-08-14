/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <TestRunner/TestRunner.h>
#include <Typography/Types.h>

TEST(TypesTest, RangeTest) {
  rl::type::TextRange range;
  range.start = 0;
  range.length = 0;
  ASSERT_FALSE(range.isIndexInRange(0));
  ASSERT_FALSE(range.isIndexInRange(1));
  ASSERT_FALSE(range.isIndexInRange(2));

  range.start = 1;
  range.length = 0;
  ASSERT_FALSE(range.isIndexInRange(0));
  ASSERT_FALSE(range.isIndexInRange(1));
  ASSERT_FALSE(range.isIndexInRange(2));
  ASSERT_FALSE(range.isIndexInRange(3));

  range.start = 0;
  range.length = 1;
  ASSERT_TRUE(range.isIndexInRange(0));
  ASSERT_FALSE(range.isIndexInRange(1));
  ASSERT_FALSE(range.isIndexInRange(2));
  ASSERT_FALSE(range.isIndexInRange(3));

  range.start = 1;
  range.length = 1;
  ASSERT_FALSE(range.isIndexInRange(0));
  ASSERT_TRUE(range.isIndexInRange(1));
  ASSERT_FALSE(range.isIndexInRange(2));
  ASSERT_FALSE(range.isIndexInRange(3));

  range.start = 1;
  range.length = 2;
  ASSERT_FALSE(range.isIndexInRange(0));
  ASSERT_TRUE(range.isIndexInRange(1));
  ASSERT_TRUE(range.isIndexInRange(2));
  ASSERT_FALSE(range.isIndexInRange(3));

  range.start = 0;
  range.length = 2;
  ASSERT_TRUE(range.isIndexInRange(0));
  ASSERT_TRUE(range.isIndexInRange(1));
  ASSERT_FALSE(range.isIndexInRange(2));
  ASSERT_FALSE(range.isIndexInRange(3));

  range.start = 0;
  range.length = 3;
  ASSERT_TRUE(range.isIndexInRange(0));
  ASSERT_TRUE(range.isIndexInRange(1));
  ASSERT_TRUE(range.isIndexInRange(2));
  ASSERT_FALSE(range.isIndexInRange(3));
}
