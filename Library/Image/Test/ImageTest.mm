// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <TestRunner/TestRunner.h>
#include <Image/Image.h>

TEST(ImageTest, SimgeDecoderJPG) {
  rl::core::File file(rl::core::URI{"file://Beachball.jpg"});

  rl::image::Image image(std::move(file));

  auto res = image.decode();

  ASSERT_TRUE(res.wasSuccessful());
  ASSERT_EQ(res.components(), rl::image::Components::RGB);
  ASSERT_EQ(res.size().width, 177);
  ASSERT_EQ(res.size().height, 177);
  ASSERT_TRUE(res.allocation().isReady());
}

TEST(ImageTest, SimgeDecoderPNG) {
  rl::core::File file(rl::core::URI{"file://Beachball.png"});

  rl::image::Image image(std::move(file));

  auto res = image.decode();

  ASSERT_TRUE(res.wasSuccessful());
  ASSERT_EQ(res.components(), rl::image::Components::RGBA);
  ASSERT_EQ(res.size().width, 177);
  ASSERT_EQ(res.size().height, 177);
  ASSERT_TRUE(res.allocation().isReady());
}

TEST(ImageTest, SimgeDecoderJPGFromAllocation) {
  rl::core::File file(rl::core::URI{"file://Beachball.jpg"});
  auto map = file.map();

  rl::core::Allocation allocation;
  ASSERT_TRUE(allocation.resize(map.size()));
  memmove(allocation.data(), map.mapping(), map.size());

  rl::image::Image image(std::move(allocation));

  auto res = image.decode();

  ASSERT_TRUE(res.wasSuccessful());
  ASSERT_EQ(res.components(), rl::image::Components::RGB);
  ASSERT_EQ(res.size().width, 177);
  ASSERT_EQ(res.size().height, 177);
  ASSERT_TRUE(res.allocation().isReady());
}

TEST(ImageTest, SimgeDecoderPNGFromAllocation) {
  rl::core::File file(rl::core::URI{"file://Beachball.png"});
  auto map = file.map();

  rl::core::Allocation allocation;
  ASSERT_TRUE(allocation.resize(map.size()));
  memmove(allocation.data(), map.mapping(), map.size());

  rl::image::Image image(std::move(allocation));

  auto res = image.decode();

  ASSERT_TRUE(res.wasSuccessful());
  ASSERT_EQ(res.components(), rl::image::Components::RGBA);
  ASSERT_EQ(res.size().width, 177);
  ASSERT_EQ(res.size().height, 177);
  ASSERT_TRUE(res.allocation().isReady());
}
