// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Namespace.h>
#include <TestRunner/TestRunner.h>

TEST(NamespaceTest, OnlyHandlesLocal) {
  rl::core::Namespace ns;

  auto h1 = rl::core::Name{ns};
  auto h2 = rl::core::Name{ns};
  auto h3 = rl::core::Name{ns};

  ASSERT_NE(h1, h2);
  ASSERT_NE(h2, h3);
  ASSERT_NE(h3, h1);
  ASSERT_EQ(ns.mappedNamesCount(), 0);
}

TEST(NamespaceTest, FullNamesLocal) {
  rl::core::Namespace ns;

  {
    auto h1 = rl::core::Name{ns};
    auto h2 = rl::core::Name{ns};
    auto h3 = rl::core::Name{ns};

    ASSERT_NE(h1, h2);
    ASSERT_NE(h2, h3);
    ASSERT_NE(h3, h1);
  }

  ASSERT_EQ(ns.mappedNamesCount(), 0);
}

TEST(NamespaceTest, NamesWithCounterparts) {
  rl::core::Namespace ns;

  rl::core::Name cp1(1, nullptr);
  rl::core::Name cp2(2, nullptr);
  rl::core::Name cp3(3, nullptr);

  auto local1 = rl::core::Name{*cp1.handle(), ns};
  auto local2 = rl::core::Name{*cp2.handle(), ns};
  auto local3 = rl::core::Name{*cp3.handle(), ns};

  ASSERT_EQ(ns.mappedNamesCount(), 3);
}

TEST(NamespaceTest, NamesWithCounterpartsWithRepeat) {
  rl::core::Namespace ns;

  rl::core::Name cp1(1, nullptr);
  rl::core::Name cp2(2, nullptr);
  rl::core::Name cp3(3, nullptr);

  auto local1 = rl::core::Name{*cp1.handle(), ns};
  auto local2 = rl::core::Name{*cp2.handle(), ns};
  auto local3 = rl::core::Name{*cp3.handle(), ns};

  ASSERT_EQ(ns.mappedNamesCount(), 3);

  auto local4 = rl::core::Name{*cp1.handle(), ns};
  auto local5 = rl::core::Name{*cp2.handle(), ns};
  auto local6 = rl::core::Name{*cp3.handle(), ns};

  ASSERT_EQ(ns.mappedNamesCount(), 3);

  ASSERT_EQ(local1, local4);
  ASSERT_EQ(local2, local5);
  ASSERT_EQ(local3, local6);
}

TEST(NamespaceTest, NamesWithCounterpartsWithRepeatSame) {
  rl::core::Namespace ns;

  rl::core::Name cp1(1, nullptr);

  auto local1 = rl::core::Name{*cp1.handle(), ns};
  auto local2 = rl::core::Name{*cp1.handle(), ns};
  auto local3 = rl::core::Name{*cp1.handle(), ns};
  auto local4 = rl::core::Name{*cp1.handle(), ns};
  auto local5 = rl::core::Name{*cp1.handle(), ns};
  auto local6 = rl::core::Name{*cp1.handle(), ns};
  auto local7 = rl::core::Name{*cp1.handle(), ns};
  auto local8 = rl::core::Name{*cp1.handle(), ns};
  auto local9 = rl::core::Name{*cp1.handle(), ns};
  auto local10 = rl::core::Name{*cp1.handle(), ns};

  ASSERT_EQ(ns.mappedNamesCount(), 1);
  ASSERT_EQ(local1.handle(), local10.handle());
}

TEST(NamespaceTest, NamesWithCounterpartsWithDeaths) {
  rl::core::Namespace ns;

  rl::core::Name cp1(1, nullptr);
  rl::core::Name cp2(2, nullptr);
  rl::core::Name cp3(3, nullptr);
  rl::core::Name cp4(4, nullptr);
  rl::core::Name cp5(5, nullptr);
  rl::core::Name cp6(6, nullptr);
  rl::core::Name cp7(7, nullptr);
  rl::core::Name cp8(8, nullptr);
  rl::core::Name cp9(9, nullptr);
  rl::core::Name cp10(10, nullptr);

  auto local1 = rl::core::Name{*cp1.handle(), ns};
  auto local2 = rl::core::Name{*cp2.handle(), ns};
  auto local3 = rl::core::Name{*cp3.handle(), ns};
  auto local4 = rl::core::Name{*cp4.handle(), ns};
  auto local5 = rl::core::Name{*cp5.handle(), ns};

  ASSERT_EQ(ns.mappedNamesCount(), 5);

  {
    auto local6 = rl::core::Name{*cp6.handle(), ns};
    auto local7 = rl::core::Name{*cp7.handle(), ns};
    auto local8 = rl::core::Name{*cp8.handle(), ns};
    auto local9 = rl::core::Name{*cp9.handle(), ns};
    auto local10 = rl::core::Name{*cp10.handle(), ns};

    ASSERT_EQ(ns.mappedNamesCount(), 10);
  }

  ASSERT_EQ(ns.mappedNamesCount(), 5);
}

TEST(NamespaceTest, AliasedHandleDies) {
  rl::core::Namespace ns;

  rl::core::Name cp(1, nullptr);

  auto local1 = rl::core::Name{*cp.handle(), ns};

  {
    auto local2 = rl::core::Name{*cp.handle(), ns};
    ASSERT_EQ(local1, local2);
  }

  auto local3 = rl::core::Name{*cp.handle(), ns};
  ASSERT_EQ(local1, local3);
}

TEST(NamespaceTest, AliasedHandleDiesWithNameCopy) {
  rl::core::Namespace ns;

  rl::core::Name cp(1, nullptr);

  auto local1 = rl::core::Name{*cp.handle(), ns};

  {
    auto local2(local1);
    ASSERT_EQ(local1, local2);
  }

  auto local3 = rl::core::Name{*cp.handle(), ns};
  ASSERT_EQ(local1, local3);
}

TEST(NamespaceTest, AliasedHandleDiesWithNameCopy2) {
  rl::core::Namespace ns;

  rl::core::Name cp(1, nullptr);

  auto local1 = rl::core::Name{*cp.handle(), ns};

  {
    auto local2(local1);
    ASSERT_EQ(local1, local2);
  }

  auto local3(local1);
  ASSERT_EQ(local1, local3);
}

TEST(NamespaceTest, AliasedHandleDiesWithNameLoseAllReferences) {
  rl::core::Namespace ns;

  rl::core::Name cp(1, nullptr);

  auto local1Handle = rl::core::DeadHandle;

  {
    auto local1 = rl::core::Name{*cp.handle(), ns};
    local1Handle = *local1.handle();

    {
      auto local2 = rl::core::Name{*cp.handle(), ns};
      ASSERT_EQ(local1, local2);
    }

    auto local3 = rl::core::Name{*cp.handle(), ns};
    ASSERT_EQ(local1, local3);
  }

  ASSERT_NE(local1Handle, rl::core::DeadHandle);

  auto local2Handle = rl::core::DeadHandle;

  {
    auto local1 = rl::core::Name{*cp.handle(), ns};
    local2Handle = *local1.handle();

    {
      auto local2 = rl::core::Name{*cp.handle(), ns};
      ASSERT_EQ(local1, local2);
    }

    auto local3 = rl::core::Name{*cp.handle(), ns};
    ASSERT_EQ(local1, local3);
  }

  ASSERT_NE(local1Handle, rl::core::DeadHandle);
  ASSERT_NE(local2Handle, rl::core::DeadHandle);

  /*
   *  Since all references were lost, we should get a fresh handle for the
   *  second set of references
   */
  ASSERT_NE(local1Handle, local2Handle);
}
