/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Compositor/Context.h>
#include <GraphicsTestRunner/GraphicsTestRunner.h>

TEST_F(GraphicsTest, SimpleUnusedContext) {
  rl::compositor::Context context;

  ASSERT_TRUE(context.dispose());

  /*
   *  Just checking that an unsued context does not assert out.
   */
}
