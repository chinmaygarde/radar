/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Image/Image.h>
#include <TestRunner/TestRunner.h>

TEST(ImageTest, SimgeDecoderJPG) {
  rl::core::FileHandle file(rl::core::URI{"file://Beachball.jpg"});
  ASSERT_TRUE(file.isValid());

  rl::image::Image image(std::move(file));

  auto res = image.decode();

  ASSERT_TRUE(res.wasSuccessful());
  ASSERT_EQ(res.components(), rl::image::ImageResult::Components::RGB);
  ASSERT_EQ(res.size().width, 177);
  ASSERT_EQ(res.size().height, 177);
  ASSERT_TRUE(res.allocation().isReady());
}

TEST(ImageTest, SimgeDecoderPNG) {
  rl::core::FileHandle file(rl::core::URI{"file://Beachball.png"});
  ASSERT_TRUE(file.isValid());

  rl::image::Image image(std::move(file));

  auto res = image.decode();

  ASSERT_TRUE(res.wasSuccessful());
  ASSERT_EQ(res.components(), rl::image::ImageResult::Components::RGBA);
  ASSERT_EQ(res.size().width, 177);
  ASSERT_EQ(res.size().height, 177);
  ASSERT_TRUE(res.allocation().isReady());
}

TEST(ImageTest, SimgeDecoderJPGFromAllocation) {
  rl::core::FileHandle file(rl::core::URI{"file://Beachball.jpg"});
  ASSERT_TRUE(file.isValid());

  rl::core::FileMapping map(file);

  rl::core::Allocation allocation;
  ASSERT_TRUE(allocation.resize(map.size()));
  memmove(allocation.data(), map.mapping(), map.size());

  rl::image::Image image(std::move(allocation));

  auto res = image.decode();

  ASSERT_TRUE(res.wasSuccessful());
  ASSERT_EQ(res.components(), rl::image::ImageResult::Components::RGB);
  ASSERT_EQ(res.size().width, 177);
  ASSERT_EQ(res.size().height, 177);
  ASSERT_TRUE(res.allocation().isReady());
}

TEST(ImageTest, SimgeDecoderPNGFromAllocation) {
  rl::core::FileHandle file(rl::core::URI{"file://Beachball.png"});
  ASSERT_TRUE(file.isValid());

  rl::core::FileMapping map(file);

  rl::core::Allocation allocation;
  ASSERT_TRUE(allocation.resize(map.size()));
  memmove(allocation.data(), map.mapping(), map.size());

  rl::image::Image image(std::move(allocation));

  auto res = image.decode();

  ASSERT_TRUE(res.wasSuccessful());
  ASSERT_EQ(res.components(), rl::image::ImageResult::Components::RGBA);
  ASSERT_EQ(res.size().width, 177);
  ASSERT_EQ(res.size().height, 177);
  ASSERT_TRUE(res.allocation().isReady());
}
