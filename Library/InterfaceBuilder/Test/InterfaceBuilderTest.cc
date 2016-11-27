// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <gtest/gtest.h>
#include <InterfaceBuilder/InterfaceBuilderArchive.h>

TEST(InterfaceBuilderTest, Init) {
  ASSERT_TRUE(true);
}

TEST(InterfaceBuilderTest, InvalidSVG) {
  rl::core::URI fileURI("file://invalid.svg");
  auto archive = rl::ib::InterfaceBuilderArchive::Make(std::move(fileURI));
  ASSERT_TRUE(archive);
  ASSERT_TRUE(archive->isValid());
}
