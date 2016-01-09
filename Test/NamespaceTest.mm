// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "RadarTest.h"

#include <Core/Core.h>

RL_DECLARE_TEST_START(NamespaceTest)

TEST(NamespaceTest, OnlyHandlesLocal) {
  rl::core::Namespace ns;

  auto h1 = ns.createHandle();
  auto h2 = ns.createHandle();
  auto h3 = ns.createHandle();

  ASSERT_NE(h1, h2);
  ASSERT_NE(h2, h3);
  ASSERT_NE(h3, h1);
  ASSERT_EQ(ns.mappedNamesCount(), 0);
}

TEST(NamespaceTest, FullNamesLocal) {
  rl::core::Namespace ns;

  {
    auto h1 = ns.create();
    auto h2 = ns.create();
    auto h3 = ns.create();

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

  auto local1 = ns.create(cp1.handle());
  auto local2 = ns.create(cp2.handle());
  auto local3 = ns.create(cp3.handle());

  ASSERT_EQ(ns.mappedNamesCount(), 3);
}

TEST(NamespaceTest, NamesWithCounterpartsWithRepeat) {
  rl::core::Namespace ns;

  rl::core::Name cp1(1, nullptr);
  rl::core::Name cp2(2, nullptr);
  rl::core::Name cp3(3, nullptr);

  auto local1 = ns.create(cp1.handle());
  auto local2 = ns.create(cp2.handle());
  auto local3 = ns.create(cp3.handle());

  ASSERT_EQ(ns.mappedNamesCount(), 3);

  auto local4 = ns.create(cp1.handle());
  auto local5 = ns.create(cp2.handle());
  auto local6 = ns.create(cp3.handle());

  ASSERT_EQ(ns.mappedNamesCount(), 3);

  ASSERT_EQ(local1, local4);
  ASSERT_EQ(local2, local5);
  ASSERT_EQ(local3, local6);
}

TEST(NamespaceTest, NamesWithCounterpartsWithRepeatSame) {
  rl::core::Namespace ns;

  rl::core::Name cp1(1, nullptr);

  auto local1 = ns.create(cp1.handle());
  auto local2 = ns.create(cp1.handle());
  auto local3 = ns.create(cp1.handle());
  auto local4 = ns.create(cp1.handle());
  auto local5 = ns.create(cp1.handle());
  auto local6 = ns.create(cp1.handle());
  auto local7 = ns.create(cp1.handle());
  auto local8 = ns.create(cp1.handle());
  auto local9 = ns.create(cp1.handle());
  auto local10 = ns.create(cp1.handle());

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

  auto local1 = ns.create(cp1.handle());
  auto local2 = ns.create(cp2.handle());
  auto local3 = ns.create(cp3.handle());
  auto local4 = ns.create(cp4.handle());
  auto local5 = ns.create(cp5.handle());

  ASSERT_EQ(ns.mappedNamesCount(), 5);

  {
    auto local6 = ns.create(cp6.handle());
    auto local7 = ns.create(cp7.handle());
    auto local8 = ns.create(cp8.handle());
    auto local9 = ns.create(cp9.handle());
    auto local10 = ns.create(cp10.handle());

    ASSERT_EQ(ns.mappedNamesCount(), 10);
  }

  ASSERT_EQ(ns.mappedNamesCount(), 5);
}

RL_DECLARE_TEST_END
