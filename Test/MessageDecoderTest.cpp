// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <stdio.h>
#include <gtest/gtest.h>

#include "Message.h"

TEST(MessageDecoderTest, SimpleInitialization) {
  rl::Message message;

  uint32_t val = 40;

  message.encode(val);

  val = 222;

  rl::Message decoder(message.data(), message.size());

  ASSERT_TRUE(decoder.decode(val));
  ASSERT_TRUE(val == 40);
}

TEST(MessageDecoderTest, MutltipleInitialization) {
  rl::Message message;

  uint32_t val = 40;
  bool foo = true;
  float a = 3.0f;
  double b = 20.0;
  char c = 'c';

  message.encode(val);
  message.encode(foo);
  message.encode(a);
  message.encode(b);
  message.encode(c);

  val = 0;
  foo = false;
  a = 0.0f;
  b = 0.0;
  c = 'd';

  rl::Message decoder(message.data(), message.size());

  ASSERT_TRUE(decoder.decode(val));
  ASSERT_TRUE(decoder.decode(foo));
  ASSERT_TRUE(decoder.decode(a));
  ASSERT_TRUE(decoder.decode(b));
  ASSERT_TRUE(decoder.decode(c));

  ASSERT_FALSE(decoder.decode(val));

  ASSERT_TRUE(val == 40);
  ASSERT_TRUE(foo == true);
  ASSERT_TRUE(a == 3.0f);
  ASSERT_TRUE(b == 20.0);
  ASSERT_TRUE(c == 'c');
}
