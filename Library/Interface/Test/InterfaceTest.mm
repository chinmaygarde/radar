// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <TestRunner/TestRunner.h>

#include <Shell/Shell.h>
#include <Coordinator/RenderSurface.h>
#include <Interface/Interface.h>

class NullRenderSurface : public rl::coordinator::RenderSurface {
 public:
  NullRenderSurface() {}

  bool makeCurrent() override { return false; }

  bool present() override { return false; }

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(NullRenderSurface);
};

class InterfaceTest : public ::testing::Test {
 public:
  InterfaceTest()
      : _renderSurface(std::make_shared<NullRenderSurface>()),
        _shell(
            rl::shell::Shell::CreateWithCurrentThreadAsHost(_renderSurface)) {}

  ~InterfaceTest() { _shell->shutdown(); }

  std::shared_ptr<rl::shell::Shell> shell() { return _shell; }

 private:
  std::shared_ptr<NullRenderSurface> _renderSurface;
  std::shared_ptr<rl::shell::Shell> _shell;
};

TEST_F(InterfaceTest, SimpleFixtureInitialization) {
  ASSERT_NE(shell(), nullptr);
}
