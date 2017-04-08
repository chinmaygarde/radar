/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

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
