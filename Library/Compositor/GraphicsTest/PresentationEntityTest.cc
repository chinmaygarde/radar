/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Compositor/BackendPass.h>
#include <Compositor/PresentationEntity.h>
#include <Geometry/PathBuilder.h>
#include <GraphicsTestRunner/GraphicsTestRunner.h>
#include "GraphicsTestFrame.h"

TEST_F(GraphicsTestFrame, ColoredBoxPrimitive) {
  /*
   *  Setup the entity.
   */
  rl::compositor::PresentationEntity box({});

  box.setFrame({10, 10, 100, 100});
  box.setBackgroundColor(rl::entity::Color::Blue());

  /*
   *  Setup the frontend pass.
   */
  rl::compositor::FrontEndPass frontEndPass;
  box.render(frontEndPass, {});

  /*
   *  Setup the backend pass.
   */
  rl::compositor::BackEndPass backEndPass;
  backEndPass.addFrontEndPass(std::move(frontEndPass));

  ASSERT_TRUE(backEndPass.render(frame(), nullptr));

  ASSERT_TRUE(snapshot());
}

TEST_F(GraphicsTestFrame, ColoredPathPrimitiveTest) {
  /*
   *  Setup the entity.
   */
  rl::geom::PathBuilder builder;
  builder.addRoundedRect({{0, 0}, {100, 150}}, {80, 20, 50, 20});

  rl::compositor::PresentationEntity box({});
  box.setPath(builder.path());
  box.setBackgroundColor(rl::entity::Color::White());

  /*
   *  Setup the frontend pass.
   */
  rl::compositor::FrontEndPass frontEndPass;
  box.render(frontEndPass, {});

  /*
   *  Setup the backend pass.
   */
  rl::compositor::BackEndPass backEndPass;
  backEndPass.addFrontEndPass(std::move(frontEndPass));

  ASSERT_TRUE(backEndPass.render(frame(), nullptr));

  ASSERT_TRUE(snapshot());
}
