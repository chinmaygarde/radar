// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <stdio.h>
#include <gtest/gtest.h>

#include <Core/Message.h>

TEST(MessageEncoderTest, SimpleInitialization) {
  rl::core::Message message;

  ASSERT_TRUE(message.encode(30.0));

  ASSERT_TRUE(message.size() == sizeof(double));
}

TEST(MessageEncoderTest, MultipleInitialization) {
  rl::core::Message message;

  size_t s = 0;
  ASSERT_TRUE(message.encode(30.0));
  ASSERT_TRUE(message.size() - sizeof(double) == s);
  s += sizeof(double);

  ASSERT_TRUE(message.encode(true));
  ASSERT_TRUE(message.size() - sizeof(bool) == s);
  s += sizeof(bool);

  ASSERT_TRUE(message.encode('c'));
  ASSERT_TRUE(message.size() - sizeof(char) == s);
  s += sizeof(char);

  ASSERT_TRUE(message.encode(50.0f));
  ASSERT_TRUE(message.size() - sizeof(float) == s);
}
