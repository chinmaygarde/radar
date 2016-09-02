// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <GraphicsTestRunner/GraphicsTestRunner.h>

#include <Compositor/Context.h>

TEST_F(GraphicsTest, SimpleUnusedContext) {
  rl::compositor::Context context;

  ASSERT_TRUE(context.dispose());

  /*
   *  Just checking that an unsued context does not assert out.
   */
}
