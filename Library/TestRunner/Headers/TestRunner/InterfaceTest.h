// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <Core/Macros.h>
#include <Shell/Shell.h>
#include <gtest/gtest.h>

/**
 * In the global namespace because this class is meant to be used as a gtest
 * fixture.
 */

class NullRenderSurface;

class InterfaceTest : public ::testing::Test {
 public:
  InterfaceTest();

  ~InterfaceTest();

  rl::shell::Shell& currentShell();

  void testOnActive(
      rl::interface::DefaultInterfaceDelegate::InterfaceCallback callback);

 private:
  std::shared_ptr<NullRenderSurface> _renderSurface;
  std::shared_ptr<rl::shell::Shell> _shell;

  RL_DISALLOW_COPY_AND_ASSIGN(InterfaceTest);
};
