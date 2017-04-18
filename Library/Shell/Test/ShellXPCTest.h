/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Shell/Shell.h>
#include <gtest/gtest.h>
#include <unistd.h>

class ShellXPCTest : public ::testing::Test {
 public:
  ShellXPCTest();

  ~ShellXPCTest();

  bool isClientContext() const;

 protected:
  void SetUp() override;

  void TearDown() override;

 private:
  pid_t _childPID;
  rl::core::EventLoopThread _shellThread;
  std::unique_ptr<rl::shell::Shell> _shell;

  void setupShell();

  void teardownShell();

  void setupChildProcess();

  void teardownChildProcess();

  RL_DISALLOW_COPY_AND_ASSIGN(ShellXPCTest);
};
