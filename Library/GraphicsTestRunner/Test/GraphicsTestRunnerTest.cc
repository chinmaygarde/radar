// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2.h>

#include <GraphicsTestRunner/GraphicsTestRunner.h>

#include "GraphicsConnection.h"

TEST(GraphicsTestRunnerTest, SimpleConnectionIsValid) {
  rl::testrunner::GraphicsConnection connection;
  ASSERT_TRUE(connection.isValid());
}

TEST(GraphicsTestRunnerTest, SimpleConnectionActivateDeactivate) {
  rl::testrunner::GraphicsConnection connection;
  ASSERT_TRUE(connection.isValid());
  ASSERT_TRUE(connection.activate());
  ASSERT_TRUE(connection.deactivate());
}

TEST_F(GraphicsTest, SimpleSetup) {
  // Just getting here means the fixture set itself up correctly.
  ASSERT_TRUE(true);
}

TEST_F(GraphicsTest, SimpleSetupWithError) {
  ASSERT_TRUE(glGetError() == GL_NO_ERROR);
  glBindBuffer(GL_GREEN_BITS, 88);
  ASSERT_TRUE(glGetError() != GL_NO_ERROR);
  ASSERT_TRUE(glGetError() == GL_NO_ERROR);
}

TEST_F(GraphicsTest, SimpleSetupClearColor) {
  ASSERT_TRUE(glGetError() == GL_NO_ERROR);

  glClearColor(1.0, 1.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  ASSERT_TRUE(glGetError() == GL_NO_ERROR);
}

TEST_F(GraphicsTest, SimpleSnapshot) {
  ASSERT_TRUE(glGetError() == GL_NO_ERROR);

  glClearColor(1.0, 1.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  ASSERT_TRUE(glGetError() == GL_NO_ERROR);

  auto captured = snapshot({0, 0, 5, 5});
  ASSERT_TRUE(captured.wasSuccessful());
  ASSERT_EQ(captured.size().width, 5);
  ASSERT_EQ(captured.size().height, 5);
}
