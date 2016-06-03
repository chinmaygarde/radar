// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <TestRunner/TestRunner.h>
#include <Image/Image.h>

#include <SampleJPG.h>
#include <SamplePNG.h>

RL_DECLARE_TEST_START(ImageDecoderTest)

TEST(ImageDecoderTest, SimpleDecoderJPG) {
  rl::core::Allocation allocation;

  ASSERT_TRUE(allocation.resize(SampleJPGLength));
  memcpy(allocation.data(), SampleJPG, SampleJPGLength);

  rl::image::Image image(std::move(allocation));

  auto res = image.decode();

  ASSERT_TRUE(res.wasSuccessful());
  ASSERT_EQ(res.components(), rl::image::Components::RGB);
  ASSERT_EQ(res.size().width, 313);
  ASSERT_EQ(res.size().height, 234.0);
  ASSERT_TRUE(res.allocation().isReady());
}

TEST(ImageDecoderTest, SimpleDecoderPNG) {
  rl::core::Allocation allocation;

  ASSERT_TRUE(allocation.resize(SamplePNGLength));
  memcpy(allocation.data(), SamplePNG, SamplePNGLength);

  rl::image::Image image(std::move(allocation));

  auto res = image.decode();

  ASSERT_TRUE(res.wasSuccessful());
  ASSERT_EQ(res.components(), rl::image::Components::RGBA);
  ASSERT_EQ(res.size().width, 800.0);
  ASSERT_EQ(res.size().height, 600.0);
  ASSERT_TRUE(res.allocation().isReady());
}

RL_DECLARE_TEST_END
