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

TEST(InterfaceBuilderTest, Rect01) {
  rl::core::URI fileURI("file://rect01.svg");
  auto archive = rl::ib::InterfaceBuilderArchive::Make(std::move(fileURI));
  ASSERT_TRUE(archive);
  ASSERT_TRUE(archive->isValid());
}

TEST_F(InterfaceTest, Rect02) {
  testOnActive([](rl::interface::Interface& interface) {
    rl::core::URI fileURI("file://rect01.svg");
    auto archive = rl::ib::InterfaceBuilderArchive::Make(std::move(fileURI));
    ASSERT_TRUE(archive);
    ASSERT_TRUE(archive->isValid());
    ASSERT_TRUE(archive->inflate(interface));
    ASSERT_TRUE(archive->inflate(interface, interface.rootEntity()));
  });
}

TEST_F(InterfaceTest, Rect02Viewbox) {
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

TEST_F(InterfaceTest, Line01CountAllChildren) {
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

TEST_F(InterfaceTest, Use01) {
  testOnActive([](rl::interface::Interface& interface) {
    rl::core::URI fileURI("file://use01.svg");
    auto archive = rl::ib::InterfaceBuilderArchive::Make(std::move(fileURI));
    ASSERT_TRUE(archive);
    ASSERT_TRUE(archive->isValid());
    ASSERT_TRUE(archive->inflate(interface));
    rl::geom::Rect expectedBounds(0, 0, 100, 30);
    ASSERT_EQ(interface.rootEntity().frame(), expectedBounds);
    ASSERT_EQ(interface.rootEntity().children().size(), 2);

    /*
     *  The frame is constructed from the values of multiple nodes. So it is
     *  important to check and get right.
     */
    rl::geom::Rect expectedFrameOfUse(20, 10, 60, 10);
    ASSERT_EQ(interface.rootEntity().children()[1]->frame(),
              expectedFrameOfUse);
  });
}

TEST_F(InterfaceTest, SketchAndroidExport) {
  testOnActive([](rl::interface::Interface& interface) {
    rl::core::URI fileURI("file://SketchAndroid.svg");
    auto archive = rl::ib::InterfaceBuilderArchive::Make(std::move(fileURI));
    ASSERT_TRUE(archive);
    ASSERT_TRUE(archive->isValid());
    ASSERT_TRUE(archive->inflate(interface));
  });
}

TEST_F(InterfaceTest, Polyline01) {
  testOnActive([](rl::interface::Interface& interface) {
    rl::core::URI fileURI("file://polyline01.svg");
    auto archive = rl::ib::InterfaceBuilderArchive::Make(std::move(fileURI));
    ASSERT_TRUE(archive);
    ASSERT_TRUE(archive->isValid());
    ASSERT_TRUE(archive->inflate(interface));
    ASSERT_EQ(interface.rootEntity().children().size(), 2);
    auto polylineEntity = interface.rootEntity().children()[1];
    ASSERT_NE(polylineEntity->path().componentCount(), 0);
    ASSERT_EQ(polylineEntity->path().componentCount(), 22);
    rl::geom::LinearPathComponent linear;
    ASSERT_TRUE(polylineEntity->path().linearComponentAtIndex(20, linear));
    rl::geom::Point expected(1050, 375);
    ASSERT_EQ(linear.p2, expected);
  });
}
