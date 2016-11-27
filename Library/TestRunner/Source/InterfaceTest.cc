// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <TestRunner/InterfaceTest.h>

class NullRenderSurface : public rl::coordinator::RenderSurface {
 public:
  NullRenderSurface() {}

  bool makeCurrent() override { return false; }

  bool present() override { return false; }

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(NullRenderSurface);
};

InterfaceTest::InterfaceTest()
    : _renderSurface(std::make_shared<NullRenderSurface>()),
      _shell(rl::shell::Shell::CreateWithCurrentThreadAsHost(_renderSurface)) {}

InterfaceTest::~InterfaceTest() {
  _shell->shutdown();
}

rl::shell::Shell& InterfaceTest::currentShell() {
  RL_ASSERT(_shell != nullptr);
  return *_shell;
}
