// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/Frame.h>
#include <GraphicsTestRunner/GraphicsTestRunner.h>

TEST_F(GraphicsTest, SimpleFrame) {
  rl::compositor::Context context;
  rl::compositor::Frame frame({100, 100}, context);

  ASSERT_TRUE(frame.isReady());

  ASSERT_TRUE(frame.begin());

  ASSERT_TRUE(frame.end());

  ASSERT_TRUE(context.dispose());
}
