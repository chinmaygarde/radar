/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/Namespace.h>
#include <TestRunner/TestRunner.h>

namespace rl {
namespace core {
namespace testing {

TEST(NamespaceTest, OnlyHandlesLocal) {
  Namespace ns;

  auto h1 = Name{ns};
  auto h2 = Name{ns};
  auto h3 = Name{ns};

  ASSERT_NE(h1, h2);
  ASSERT_NE(h2, h3);
  ASSERT_NE(h3, h1);
  ASSERT_EQ(ns.mappedNamesCount(), 0u);
}

TEST(NamespaceTest, FullNamesLocal) {
  Namespace ns;

  {
    auto h1 = Name{ns};
    auto h2 = Name{ns};
    auto h3 = Name{ns};

    ASSERT_NE(h1, h2);
    ASSERT_NE(h2, h3);
    ASSERT_NE(h3, h1);
  }

  ASSERT_EQ(ns.mappedNamesCount(), 0u);
}

TEST(NamespaceTest, NamesWithCounterparts) {
  Namespace ns;

  Name cp1(1, nullptr);
  Name cp2(2, nullptr);
  Name cp3(3, nullptr);

  auto local1 = Name{*cp1.handle(), ns};
  auto local2 = Name{*cp2.handle(), ns};
  auto local3 = Name{*cp3.handle(), ns};

  ASSERT_EQ(ns.mappedNamesCount(), 3u);
}

TEST(NamespaceTest, NamesWithCounterpartsWithRepeat) {
  Namespace ns;

  Name cp1(1, nullptr);
  Name cp2(2, nullptr);
  Name cp3(3, nullptr);

  auto local1 = Name{*cp1.handle(), ns};
  auto local2 = Name{*cp2.handle(), ns};
  auto local3 = Name{*cp3.handle(), ns};

  ASSERT_EQ(ns.mappedNamesCount(), 3u);

  auto local4 = Name{*cp1.handle(), ns};
  auto local5 = Name{*cp2.handle(), ns};
  auto local6 = Name{*cp3.handle(), ns};

  ASSERT_EQ(ns.mappedNamesCount(), 3u);

  ASSERT_EQ(local1, local4);
  ASSERT_EQ(local2, local5);
  ASSERT_EQ(local3, local6);
}

TEST(NamespaceTest, NamesWithCounterpartsWithRepeatSame) {
  Namespace ns;

  Name cp1(1, nullptr);

  auto local1 = Name{*cp1.handle(), ns};
  auto local2 = Name{*cp1.handle(), ns};
  auto local3 = Name{*cp1.handle(), ns};
  auto local4 = Name{*cp1.handle(), ns};
  auto local5 = Name{*cp1.handle(), ns};
  auto local6 = Name{*cp1.handle(), ns};
  auto local7 = Name{*cp1.handle(), ns};
  auto local8 = Name{*cp1.handle(), ns};
  auto local9 = Name{*cp1.handle(), ns};
  auto local10 = Name{*cp1.handle(), ns};

  ASSERT_EQ(ns.mappedNamesCount(), 1u);
  ASSERT_EQ(local1.handle(), local10.handle());
}

TEST(NamespaceTest, NamesWithCounterpartsWithDeaths) {
  Namespace ns;

  Name cp1(1, nullptr);
  Name cp2(2, nullptr);
  Name cp3(3, nullptr);
  Name cp4(4, nullptr);
  Name cp5(5, nullptr);
  Name cp6(6, nullptr);
  Name cp7(7, nullptr);
  Name cp8(8, nullptr);
  Name cp9(9, nullptr);
  Name cp10(10, nullptr);

  auto local1 = Name{*cp1.handle(), ns};
  auto local2 = Name{*cp2.handle(), ns};
  auto local3 = Name{*cp3.handle(), ns};
  auto local4 = Name{*cp4.handle(), ns};
  auto local5 = Name{*cp5.handle(), ns};

  ASSERT_EQ(ns.mappedNamesCount(), 5u);

  {
    auto local6 = Name{*cp6.handle(), ns};
    auto local7 = Name{*cp7.handle(), ns};
    auto local8 = Name{*cp8.handle(), ns};
    auto local9 = Name{*cp9.handle(), ns};
    auto local10 = Name{*cp10.handle(), ns};

    ASSERT_EQ(ns.mappedNamesCount(), 10u);
  }

  ASSERT_EQ(ns.mappedNamesCount(), 5u);
}

TEST(NamespaceTest, AliasedHandleDies) {
  Namespace ns;

  Name cp(1, nullptr);

  auto local1 = Name{*cp.handle(), ns};

  {
    auto local2 = Name{*cp.handle(), ns};
    ASSERT_EQ(local1, local2);
  }

  auto local3 = Name{*cp.handle(), ns};
  ASSERT_EQ(local1, local3);
}

TEST(NamespaceTest, AliasedHandleDiesWithNameCopy) {
  Namespace ns;

  Name cp(1, nullptr);

  auto local1 = Name{*cp.handle(), ns};

  {
    auto local2(local1);
    ASSERT_EQ(local1, local2);
  }

  auto local3 = Name{*cp.handle(), ns};
  ASSERT_EQ(local1, local3);
}

TEST(NamespaceTest, AliasedHandleDiesWithNameCopy2) {
  Namespace ns;

  Name cp(1, nullptr);

  auto local1 = Name{*cp.handle(), ns};

  {
    auto local2(local1);
    ASSERT_EQ(local1, local2);
  }

  auto local3(local1);
  ASSERT_EQ(local1, local3);
}

TEST(NamespaceTest, AliasedHandleDiesWithNameLoseAllReferences) {
  Namespace ns;

  Name cp(1, nullptr);

  auto local1Handle = DeadHandle;

  {
    auto local1 = Name{*cp.handle(), ns};
    local1Handle = *local1.handle();

    {
      auto local2 = Name{*cp.handle(), ns};
      ASSERT_EQ(local1, local2);
    }

    auto local3 = Name{*cp.handle(), ns};
    ASSERT_EQ(local1, local3);
  }

  ASSERT_NE(local1Handle, DeadHandle);

  auto local2Handle = DeadHandle;

  {
    auto local1 = Name{*cp.handle(), ns};
    local2Handle = *local1.handle();

    {
      auto local2 = Name{*cp.handle(), ns};
      ASSERT_EQ(local1, local2);
    }

    auto local3 = Name{*cp.handle(), ns};
    ASSERT_EQ(local1, local3);
  }

  ASSERT_NE(local1Handle, DeadHandle);
  ASSERT_NE(local2Handle, DeadHandle);

  /*
   *  Since all references were lost, we should get a fresh handle for the
   *  second set of references
   */
  ASSERT_NE(local1Handle, local2Handle);
}

}  // namespace testing
}  // namespace core
}  // namespace rl
