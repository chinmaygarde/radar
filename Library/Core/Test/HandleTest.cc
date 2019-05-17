/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/Handle.h>
#include <TestRunner/TestRunner.h>

namespace rl {
namespace core {
namespace testing {

static size_t GDestructions = 0;

struct CustomIntHandleTraits {
  static int NullHandle() { return -1; }
  static bool IsValid(int handle) { return handle >= 0; }
  static bool CollectHandle(int handle) {
    GDestructions++;
    return true;
  }
};

using CustomIntHandle = Handle<int, CustomIntHandleTraits>;

TEST(HandleTest, SimpleValidity) {
  size_t destructions = GDestructions;
  {
    CustomIntHandle valid(22);
    ASSERT_TRUE(valid.isValid());
    ASSERT_EQ(valid.get(), 22);
    CustomIntHandle invalid;
    ASSERT_FALSE(invalid.isValid());
    ASSERT_EQ(invalid.get(), -1);
  }
  ASSERT_EQ(GDestructions, destructions + 1);
}

TEST(HandleTest, HandleCanRest) {
  size_t destructions = GDestructions;
  {
    CustomIntHandle handle(22);
    ASSERT_TRUE(handle.isValid());
    ASSERT_EQ(handle.get(), 22);
    ASSERT_TRUE(handle.reset());
    ASSERT_FALSE(handle.isValid());
    ASSERT_EQ(handle.get(), -1);
  }
  ASSERT_EQ(GDestructions, destructions + 1);
}

TEST(HandleTest, CanBeSwapped) {
  size_t destructions = GDestructions;
  {
    CustomIntHandle a(22);
    CustomIntHandle b(33);
    a.swap(b);
    ASSERT_EQ(a.get(), 33);
    ASSERT_EQ(b.get(), 22);
  }
  ASSERT_EQ(GDestructions, destructions + 2);
}

TEST(HandleTest, CanBeAssigned) {
  size_t destructions = GDestructions;
  {
    CustomIntHandle a(22);
    CustomIntHandle b(33);
    a = std::move(b);
    ASSERT_EQ(a.get(), 33);
    ASSERT_EQ(b.get(), 22);
  }
  ASSERT_EQ(GDestructions, destructions + 2);
}

TEST(HandleTest, CanBeAssignedToSame) {
  size_t destructions = GDestructions;
  {
    CustomIntHandle a(22);
    CustomIntHandle b(22);
    a = std::move(b);
    ASSERT_EQ(a.get(), 22);
    ASSERT_EQ(b.get(), -1);
  }
  ASSERT_EQ(GDestructions, destructions + 1);
}

TEST(HandleTest, CanBeMoved) {
  size_t destructions = GDestructions;
  {
    CustomIntHandle a(22);
    CustomIntHandle b(std::move(a));
    ASSERT_EQ(a.get(), -1);
    ASSERT_EQ(b.get(), 22);
  }
  ASSERT_EQ(GDestructions, destructions + 1);
}

TEST(HandleTest, Equality) {
  size_t destructions = GDestructions;
  {
    CustomIntHandle a(1);
    CustomIntHandle b(2);
    CustomIntHandle c(1);

    ASSERT_FALSE(a == b);
    ASSERT_TRUE(a != b);

    ASSERT_TRUE(a == c);
    ASSERT_FALSE(a != c);
  }
  ASSERT_EQ(GDestructions, destructions + 3);
}

}  // namespace testing
}  // namespace core
}  // namespace rl
