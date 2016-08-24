// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "GraphicsTest.h"

#include <Core/Utilities.h>

#include "GraphicsConnection.h"

GraphicsTest::GraphicsTest()
    : _connection(rl::core::make_unique<rl::testrunner::GraphicsConnection>()) {
}

GraphicsTest::~GraphicsTest() {}

void GraphicsTest::SetUp() {
  auto setup = _connection->activate();
  RL_ASSERT_MSG(setup, "Must be able to setup the graphics connection");
}

void GraphicsTest::TearDown() {
  auto setup = _connection->deactivate();
  RL_ASSERT_MSG(setup, "Must be able to tear down the graphics connection");
}
