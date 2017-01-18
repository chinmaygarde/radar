// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/Frame.h>
#include <GraphicsTestRunner/GraphicsTestRunner.h>
#include "GraphicsTestFrame.h"
#include "Primitive/ColoredBoxPrimitive.h"

TEST_F(GraphicsTest, SimpleBoxPrimitive) {
  rl::compositor::Context context;
  rl::compositor::Frame frame({320, 480}, context);

  ASSERT_TRUE(frame.begin());

  rl::compositor::ColoredBoxPrimitive primitive(rl::entity::ColorBlue);

  ASSERT_TRUE(primitive.render(frame));

  ASSERT_TRUE(frame.end());

  ASSERT_TRUE(context.dispose());
}

TEST_F(GraphicsTestFrame, SimpleBoxPrimitiveWithFixture) {
  rl::compositor::ColoredBoxPrimitive primitive(rl::entity::ColorBlue);

  ASSERT_TRUE(primitive.render(frame()));
}
