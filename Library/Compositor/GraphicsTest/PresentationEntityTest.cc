// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <GraphicsTestRunner/GraphicsTestRunner.h>
#include "GraphicsTestFrame.h"

#include <Compositor/PresentationEntity.h>
#include <Compositor/BackendPass.h>

TEST_F(GraphicsTestFrame, ColoredBoxPrimitive) {
  /*
   *  Setup the entity.
   */
  rl::compositor::PresentationEntity box({});

  box.setFrame({10, 10, 100, 100});
  box.setBackgroundColor(rl::entity::ColorBlue);

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
