/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

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
