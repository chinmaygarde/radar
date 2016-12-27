// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <TestRunner/TestRunner.h>
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
    rl::ib::InterfaceBuilderArchive::EntityMap map;
    ASSERT_TRUE(archive->inflate(interface, map) != nullptr);
  });
}

TEST_F(InterfaceTest, Rect02Viewbox) {
  testOnActive([](rl::interface::Interface& interface) {
    rl::core::URI fileURI("file://rect02.svg");
    auto archive = rl::ib::InterfaceBuilderArchive::Make(std::move(fileURI));
    ASSERT_TRUE(archive);
    ASSERT_TRUE(archive->isValid());
    rl::ib::InterfaceBuilderArchive::EntityMap map;
    auto inflated = archive->inflate(interface, map);
    ASSERT_TRUE(inflated != nullptr);
    rl::geom::Rect expectedBounds(0, 0, 1200, 400);
    ASSERT_EQ(inflated->frame(), expectedBounds);
  });
}

TEST_F(InterfaceTest, Ellipse01) {
  testOnActive([](rl::interface::Interface& interface) {
    rl::core::URI fileURI("file://ellipse01.svg");
    auto archive = rl::ib::InterfaceBuilderArchive::Make(std::move(fileURI));
    ASSERT_TRUE(archive);
    ASSERT_TRUE(archive->isValid());
    rl::ib::InterfaceBuilderArchive::EntityMap map;
    auto inflated = archive->inflate(interface, map);
    ASSERT_TRUE(inflated != nullptr);
    rl::geom::Rect expectedBounds(0, 0, 1200, 400);
    ASSERT_EQ(inflated->frame(), expectedBounds);
    ASSERT_EQ(inflated->children().size(), 3);
  });
}

TEST_F(InterfaceTest, Circle01) {
  testOnActive([](rl::interface::Interface& interface) {
    rl::core::URI fileURI("file://circle01.svg");
    auto archive = rl::ib::InterfaceBuilderArchive::Make(std::move(fileURI));
    ASSERT_TRUE(archive);
    ASSERT_TRUE(archive->isValid());
    rl::ib::InterfaceBuilderArchive::EntityMap map;
    auto inflated = archive->inflate(interface, map);
    ASSERT_TRUE(inflated != nullptr);
    rl::geom::Rect expectedBounds(0, 0, 1200, 400);
    ASSERT_EQ(inflated->frame(), expectedBounds);
    ASSERT_EQ(inflated->children().size(), 2);
  });
}

TEST_F(InterfaceTest, Polygon01) {
  testOnActive([](rl::interface::Interface& interface) {
    rl::core::URI fileURI("file://polygon01.svg");
    auto archive = rl::ib::InterfaceBuilderArchive::Make(std::move(fileURI));
    ASSERT_TRUE(archive);
    ASSERT_TRUE(archive->isValid());
    rl::ib::InterfaceBuilderArchive::EntityMap map;
    auto inflated = archive->inflate(interface, map);
    ASSERT_TRUE(inflated != nullptr);
    rl::geom::Rect expectedBounds(0, 0, 1200, 400);
    ASSERT_EQ(inflated->frame(), expectedBounds);
    ASSERT_EQ(inflated->children().size(), 3);
  });
}

TEST_F(InterfaceTest, Line01) {
  testOnActive([](rl::interface::Interface& interface) {
    rl::core::URI fileURI("file://line01.svg");
    auto archive = rl::ib::InterfaceBuilderArchive::Make(std::move(fileURI));
    ASSERT_TRUE(archive);
    ASSERT_TRUE(archive->isValid());
    rl::ib::InterfaceBuilderArchive::EntityMap map;
    auto inflated = archive->inflate(interface, map);
    ASSERT_TRUE(inflated != nullptr);
    rl::geom::Rect expectedBounds(0, 0, 1200, 400);
    ASSERT_EQ(inflated->frame(), expectedBounds);
    ASSERT_EQ(inflated->children().size(), 2);
    auto gElement = inflated->children()[1];
    ASSERT_EQ(gElement->children().size(), 5);
  });
}

TEST_F(InterfaceTest, Line01CountAllChildren) {
  testOnActive([](rl::interface::Interface& interface) {
    rl::core::URI fileURI("file://line01.svg");
    auto archive = rl::ib::InterfaceBuilderArchive::Make(std::move(fileURI));
    ASSERT_TRUE(archive);
    ASSERT_TRUE(archive->isValid());
    rl::ib::InterfaceBuilderArchive::EntityMap map;
    auto inflated = archive->inflate(interface, map);
    ASSERT_TRUE(inflated != nullptr);
    rl::geom::Rect expectedBounds(0, 0, 1200, 400);
    ASSERT_EQ(inflated->frame(), expectedBounds);

    size_t count = 0;
    inflated->visitHierarchy([&count](rl::interface::ModelEntity&) -> bool {
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
    rl::ib::InterfaceBuilderArchive::EntityMap map;
    auto inflated = archive->inflate(interface, map);
    ASSERT_TRUE(inflated != nullptr);
    rl::geom::Rect expectedBounds(0, 0, 100, 30);
    ASSERT_EQ(inflated->frame(), expectedBounds);
    ASSERT_EQ(inflated->children().size(), 2);

    /*
     *  The frame is constructed from the values of multiple nodes. So it is
     *  important to check and get right.
     */
    rl::geom::Rect expectedFrameOfUse(20, 10, 60, 10);
    ASSERT_EQ(inflated->children()[1]->frame(), expectedFrameOfUse);
  });
}

TEST_F(InterfaceTest, SketchAndroidExport) {
  testOnActive([](rl::interface::Interface& interface) {
    rl::core::URI fileURI("file://SketchAndroid.svg");
    auto archive = rl::ib::InterfaceBuilderArchive::Make(std::move(fileURI));
    ASSERT_TRUE(archive);
    ASSERT_TRUE(archive->isValid());
    rl::ib::InterfaceBuilderArchive::EntityMap map;
    ASSERT_TRUE(archive->inflate(interface, map) != nullptr);
  });
}

TEST_F(InterfaceTest, Polyline01) {
  testOnActive([](rl::interface::Interface& interface) {
    rl::core::URI fileURI("file://polyline01.svg");
    auto archive = rl::ib::InterfaceBuilderArchive::Make(std::move(fileURI));
    ASSERT_TRUE(archive);
    ASSERT_TRUE(archive->isValid());
    rl::ib::InterfaceBuilderArchive::EntityMap map;
    auto inflated = archive->inflate(interface, map);
    ASSERT_TRUE(inflated != nullptr);
    ASSERT_EQ(inflated->children().size(), 2);
    auto polylineEntity = inflated->children()[1];
    ASSERT_NE(polylineEntity->path().componentCount(), 0);
    ASSERT_EQ(polylineEntity->path().componentCount(), 22);
    rl::geom::LinearPathComponent linear;
    ASSERT_TRUE(polylineEntity->path().linearComponentAtIndex(20, linear));
    rl::geom::Point expected(1050, 375);
    ASSERT_EQ(linear.p2, expected);
  });
}

TEST_F(InterfaceTest, StarPolygon) {
  testOnActive([](rl::interface::Interface& interface) {
    rl::core::URI fileURI("file://starpolygon.svg");
    auto archive = rl::ib::InterfaceBuilderArchive::Make(std::move(fileURI));
    ASSERT_TRUE(archive);
    ASSERT_TRUE(archive->isValid());
    rl::ib::InterfaceBuilderArchive::EntityMap map;
    auto inflated = archive->inflate(interface, map);
    ASSERT_TRUE(inflated != nullptr);
    ASSERT_EQ(inflated->children().size(), 1);
    auto polylineEntity = inflated->children()[0];
    ASSERT_NE(polylineEntity->path().componentCount(), 0);
    ASSERT_EQ(polylineEntity->path().componentCount(), 76);
    rl::geom::LinearPathComponent linear;
    ASSERT_TRUE(polylineEntity->path().linearComponentAtIndex(75, linear));
    rl::geom::Point expected(156, 225);
    ASSERT_EQ(linear.p2, expected);
  });
}

TEST_F(InterfaceTest, Nighthawks) {
  testOnActive([](rl::interface::Interface& interface) {
    rl::core::URI fileURI("file://nighthawks.svg");
    auto archive = rl::ib::InterfaceBuilderArchive::Make(std::move(fileURI));
    ASSERT_TRUE(archive);
    ASSERT_TRUE(archive->isValid());
    rl::ib::InterfaceBuilderArchive::EntityMap map;
    auto inflated = archive->inflate(interface, map);
    ASSERT_TRUE(inflated != nullptr);
    ASSERT_EQ(inflated->children().size(), 1);
  });
}

TEST_F(InterfaceTest, TestTransformation) {
  testOnActive([](rl::interface::Interface& interface) {
    rl::core::URI fileURI("file://transform.svg");
    auto archive = rl::ib::InterfaceBuilderArchive::Make(std::move(fileURI));
    ASSERT_TRUE(archive);
    ASSERT_TRUE(archive->isValid());
    rl::ib::InterfaceBuilderArchive::EntityMap map;
    auto inflated = archive->inflate(interface, map);
    ASSERT_TRUE(inflated != nullptr);
    ASSERT_EQ(inflated->children().size(), 1);
    rl::geom::Matrix expected =
        rl::geom::Matrix::Translation({140.5, 102, 0.0}) *
        rl::geom::Matrix::RotationZ(36.0 * M_PI / 180.0) *
        rl::geom::Matrix::Translation({-140.5, -102.0, 0.0});
    ASSERT_MATRIX_NEAR(expected, inflated->children()[0]->transformation());
  });
}

TEST_F(InterfaceTest, Quad01) {
  testOnActive([](rl::interface::Interface& interface) {
    rl::core::URI fileURI("file://quad01.svg");
    auto archive = rl::ib::InterfaceBuilderArchive::Make(std::move(fileURI));
    ASSERT_TRUE(archive);
    ASSERT_TRUE(archive->isValid());
    rl::ib::InterfaceBuilderArchive::EntityMap map;
    auto inflated = archive->inflate(interface, map);
    ASSERT_TRUE(inflated != nullptr);
    ASSERT_EQ(inflated->children().size(), 4);
  });
}

TEST_F(InterfaceTest, SharedDialog) {
  testOnActive([](rl::interface::Interface& interface) {
    rl::core::URI fileURI("file://shareddialog.svg");
    auto archive = rl::ib::InterfaceBuilderArchive::Make(std::move(fileURI));
    ASSERT_TRUE(archive);
    ASSERT_TRUE(archive->isValid());
    rl::ib::InterfaceBuilderArchive::EntityMap map;
    auto inflated = archive->inflate(interface, map);
    ASSERT_TRUE(inflated != nullptr);
    ASSERT_NE(map.size(), 0);

    auto expected = rl::geom::Rect{0, 0, 375, 667};
    ASSERT_RECT_NEAR(inflated->frame(), expected);

    expected = rl::geom::Rect{0, 220, 375, 126};
    ASSERT_RECT_NEAR(map.at("Header")->frame(), expected);

    expected = rl::geom::Rect{0, 346, 375, 130};
    ASSERT_RECT_NEAR(map.at("Apps")->frame(), expected);

    expected = rl::geom::Rect{0, 476, 375, 116};
    ASSERT_RECT_NEAR(map.at("Actions")->frame(), expected);

    expected = rl::geom::Rect{25, 496, 60, 80};
    ASSERT_RECT_NEAR(map.at("Copy")->frame(), expected);

    expected = rl::geom::Rect{108, 496, 60, 80};
    ASSERT_RECT_NEAR(map.at("Safari")->frame(), expected);

    expected = rl::geom::Rect{191, 496, 60, 80};
    ASSERT_RECT_NEAR(map.at("Print")->frame(), expected);

    expected = rl::geom::Rect{0, 0, 375, 667};
    ASSERT_RECT_NEAR(map.at("Modal-Overlay")->frame(), expected);

    expected = rl::geom::Rect{10, 600, 355, 57};
    ASSERT_RECT_NEAR(map.at("Cancel-Background")->frame(), expected);

    expected = rl::geom::Rect{154, 616.5, 66, 24};
    ASSERT_RECT_NEAR(map.at("Cancel")->frame(), expected);

    expected = rl::geom::Rect{10, 220, 355, 372};
    ASSERT_RECT_NEAR(map.at("Sheet-Background")->frame(), expected);

    expected = rl::geom::Rect{25, 336, 60, 80};
    ASSERT_RECT_NEAR(map.at("Message")->frame(), expected);

    expected = rl::geom::Rect{108, 366, 60, 80};
    ASSERT_RECT_NEAR(map.at("Mail")->frame(), expected);

    expected = rl::geom::Rect{191, 366, 60, 80};
    ASSERT_RECT_NEAR(map.at("Twitter")->frame(), expected);

    expected = rl::geom::Rect{29, 246, 326, 52};
    ASSERT_RECT_NEAR(map.at("Icon-+-Description")->frame(), expected);
  });
}
