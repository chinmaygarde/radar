/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

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
