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

TEST(TypesTest, ContainsTest) {
  rl::type::TextRange a, b;

  a.start = 0;
  a.length = 0;
  b.start = 0;
  b.length = 0;
  ASSERT_FALSE(a.containsRange(b));

  a.start = 0;
  a.length = 1;
  b.start = 0;
  b.length = 0;
  ASSERT_FALSE(a.containsRange(b));

  a.start = 0;
  a.length = 1;
  b.start = 0;
  b.length = 1;
  ASSERT_TRUE(a.containsRange(b));

  a.start = 0;
  a.length = 1;
  b.start = 0;
  b.length = 2;
  ASSERT_FALSE(a.containsRange(b));

  a.start = 0;
  a.length = 1;
  b.start = 1;
  b.length = 1;
  ASSERT_FALSE(a.containsRange(b));

  a.start = 0;
  a.length = 1;
  b.start = 1;
  b.length = 0;
  ASSERT_FALSE(a.containsRange(b));

  a.start = 100;
  a.length = 150;
  b.start = 0;
  b.length = 50;
  ASSERT_FALSE(a.containsRange(b));

  a.start = 100;
  a.length = 150;
  b.start = 0;
  b.length = 125;
  ASSERT_FALSE(a.containsRange(b));

  a.start = 100;
  a.length = 150;
  b.start = 100;
  b.length = 125;
  ASSERT_TRUE(a.containsRange(b));

  a.start = 100;
  a.length = 150;
  b.start = 100;
  b.length = 150;
  ASSERT_TRUE(a.containsRange(b));

  a.start = 100;
  a.length = 150;
  b.start = 100;
  b.length = 151;
  ASSERT_FALSE(a.containsRange(b));

  a.start = 100;
  a.length = 150;
  b.start = 120;
  b.length = 151;
  ASSERT_FALSE(a.containsRange(b));

  a.start = 100;
  a.length = 150;
  b.start = 120;
  b.length = 150;
  ASSERT_FALSE(a.containsRange(b));

  a.start = 100;
  a.length = 150;
  b.start = 120;
  b.length = 30;
  ASSERT_TRUE(a.containsRange(b));

  a.start = 100;
  a.length = 150;
  b.start = 120;
  b.length = 31;
  ASSERT_TRUE(a.containsRange(b));

  a.start = 100;
  a.length = 30;
  b.start = 120;
  b.length = 11;
  ASSERT_FALSE(a.containsRange(b));

  a.start = 100;
  a.length = 30;
  b.start = 120;
  b.length = 10;
  ASSERT_TRUE(a.containsRange(b));

  a.start = 100;
  a.length = 30;
  b.start = 120;
  b.length = 9;
  ASSERT_TRUE(a.containsRange(b));

  a.start = 100;
  a.length = 30;
  b.start = 130;
  b.length = 1;
  ASSERT_FALSE(a.containsRange(b));

  a.start = 100;
  a.length = 30;
  b.start = 129;
  b.length = 1;
  ASSERT_TRUE(a.containsRange(b));
}
