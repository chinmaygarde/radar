// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <gtest/gtest.h>
#include <InterfaceBuilder/InterfaceBuilderArchive.h>
#include <TestRunner/InterfaceTest.h>

TEST(InterfaceBuilderTest, Init) {
  ASSERT_TRUE(true);
}

TEST(InterfaceBuilderTest, InvalidSVG) {
  rl::core::URI fileURI("file://invalid.svg");
  auto archive = rl::ib::InterfaceBuilderArchive::Make(std::move(fileURI));
  ASSERT_FALSE(archive);
}

TEST(InterfaceBuilderTest, ValidSVG) {
  rl::core::URI fileURI("file://rect01.svg");
  auto archive = rl::ib::InterfaceBuilderArchive::Make(std::move(fileURI));
  ASSERT_TRUE(archive);
  ASSERT_TRUE(archive->isValid());
}

TEST_F(InterfaceTest, ValidSVGInflate) {
  testOnActive([](rl::interface::Interface& interface) {
    rl::core::URI fileURI("file://rect01.svg");
    auto archive = rl::ib::InterfaceBuilderArchive::Make(std::move(fileURI));
    ASSERT_TRUE(archive);
    ASSERT_TRUE(archive->isValid());
    ASSERT_TRUE(archive->inflate(interface));
    ASSERT_TRUE(archive->inflate(interface, interface.rootEntity()));
  });
}

TEST_F(InterfaceTest, SVGCheckViewbox) {
  testOnActive([](rl::interface::Interface& interface) {
    rl::core::URI fileURI("file://rect02.svg");
    auto archive = rl::ib::InterfaceBuilderArchive::Make(std::move(fileURI));
    ASSERT_TRUE(archive);
    ASSERT_TRUE(archive->isValid());
    ASSERT_TRUE(archive->inflate(interface));
    rl::geom::Rect expectedBounds(0, 0, 1200, 400);
    ASSERT_EQ(interface.rootEntity().frame(), expectedBounds);
  });
}

TEST_F(InterfaceTest, Ellipse01) {
  testOnActive([](rl::interface::Interface& interface) {
    rl::core::URI fileURI("file://ellipse01.svg");
    auto archive = rl::ib::InterfaceBuilderArchive::Make(std::move(fileURI));
    ASSERT_TRUE(archive);
    ASSERT_TRUE(archive->isValid());
    ASSERT_TRUE(archive->inflate(interface));
    rl::geom::Rect expectedBounds(0, 0, 1200, 400);
    ASSERT_EQ(interface.rootEntity().frame(), expectedBounds);
    ASSERT_EQ(interface.rootEntity().children().size(), 3);
  });
}

TEST_F(InterfaceTest, Circle01) {
  testOnActive([](rl::interface::Interface& interface) {
    rl::core::URI fileURI("file://circle01.svg");
    auto archive = rl::ib::InterfaceBuilderArchive::Make(std::move(fileURI));
    ASSERT_TRUE(archive);
    ASSERT_TRUE(archive->isValid());
    ASSERT_TRUE(archive->inflate(interface));
    rl::geom::Rect expectedBounds(0, 0, 1200, 400);
    ASSERT_EQ(interface.rootEntity().frame(), expectedBounds);
    ASSERT_EQ(interface.rootEntity().children().size(), 2);
  });
}

TEST_F(InterfaceTest, Polygon01) {
  testOnActive([](rl::interface::Interface& interface) {
    rl::core::URI fileURI("file://polygon01.svg");
    auto archive = rl::ib::InterfaceBuilderArchive::Make(std::move(fileURI));
    ASSERT_TRUE(archive);
    ASSERT_TRUE(archive->isValid());
    ASSERT_TRUE(archive->inflate(interface));
    rl::geom::Rect expectedBounds(0, 0, 1200, 400);
    ASSERT_EQ(interface.rootEntity().frame(), expectedBounds);
    ASSERT_EQ(interface.rootEntity().children().size(), 3);
  });
}

TEST_F(InterfaceTest, Line01) {
  testOnActive([](rl::interface::Interface& interface) {
    rl::core::URI fileURI("file://line01.svg");
    auto archive = rl::ib::InterfaceBuilderArchive::Make(std::move(fileURI));
    ASSERT_TRUE(archive);
    ASSERT_TRUE(archive->isValid());
    ASSERT_TRUE(archive->inflate(interface));
    rl::geom::Rect expectedBounds(0, 0, 1200, 400);
    ASSERT_EQ(interface.rootEntity().frame(), expectedBounds);
    ASSERT_EQ(interface.rootEntity().children().size(), 2);
    auto gElement = interface.rootEntity().children()[1];
    ASSERT_EQ(gElement->children().size(), 5);
  });
}

TEST_F(InterfaceTest, CountAllChildren) {
  testOnActive([](rl::interface::Interface& interface) {
    rl::core::URI fileURI("file://line01.svg");
    auto archive = rl::ib::InterfaceBuilderArchive::Make(std::move(fileURI));
    ASSERT_TRUE(archive);
    ASSERT_TRUE(archive->isValid());
    ASSERT_TRUE(archive->inflate(interface));
    rl::geom::Rect expectedBounds(0, 0, 1200, 400);
    ASSERT_EQ(interface.rootEntity().frame(), expectedBounds);

    size_t count = 0;
    interface.rootEntity().visitHierarchy(
        [&count](rl::interface::ModelEntity&) -> bool {
          count++;
          return true;
        });

    ASSERT_EQ(count, 8);
  });
}
