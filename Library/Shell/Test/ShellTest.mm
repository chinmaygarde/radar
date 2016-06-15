// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <TestRunner/TestRunner.h>

#include <Shell/Shell.h>

class NullRenderSurface : public rl::coordinator::RenderSurface {
 public:
  NullRenderSurface(){};

  bool makeCurrent() override { return false; }

  bool present() override { return false; }

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(NullRenderSurface);
};

TEST(ShellTest, SimpleInitialization) {
  auto shell = rl::shell::Shell::CreateWithCurrentThreadAsHost(
      std::make_shared<NullRenderSurface>());
  ASSERT_NE(shell, nullptr);
}
