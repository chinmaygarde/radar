/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "NullRenderSurface.h"
#include "ShellXPCTest.h"

ShellXPCTest::ShellXPCTest() = default;

ShellXPCTest::~ShellXPCTest() = default;

bool ShellXPCTest::isClientContext() const {
  return _childPID == 0;
}

void ShellXPCTest::SetUp() {
  /*
   *  Setup the shell before launching any child contexts.
   */
  setupShell();

  /*
   *  Launch the child context.
   */
  setupChildProcess();
}

void ShellXPCTest::TearDown() {
  /*
   *  Make sure to tear down any child context before tearing down the shell.
   *  This simulates shell lifcycle managed by socket activation.
   */
  teardownChildProcess();

  /*
   *  Finally, tear down the shell.
   */
  teardownShell();
}

void ShellXPCTest::setupShell() {
  ASSERT_FALSE(_shell);
  rl::core::Latch latch(1);
  auto& shell = _shell;
  _shellThread.loop().get()->dispatchAsync([&latch, &shell]() {
    auto renderSurface = std::make_shared<rl::shell::NullRenderSurface>();
    shell = rl::shell::Shell::CreateWithCurrentThreadAsHost(renderSurface);
    latch.countDown();
  });
  latch.wait();
  ASSERT_TRUE(_shell);
}

void ShellXPCTest::teardownShell() {
  ASSERT_TRUE(_shell);
  rl::core::Latch latch(1);
  auto& shell = _shell;
  _shellThread.loop().get()->dispatchAsync([&latch, &shell]() {
    shell->shutdown();
    shell = nullptr;
    latch.countDown();
  });
  latch.wait();
  ASSERT_FALSE(_shell);
}

void ShellXPCTest::setupChildProcess() {
  _childPID = fork();
}

void ShellXPCTest::teardownChildProcess() {
  if (isClientContext()) {
    /*
     *  We are in the child context. That means the test has already run in the
     *  client and we can now die so that the server context can shutdown.
     */
    exit(HasFailure() ? EXIT_FAILURE : EXIT_SUCCESS);
  } else {
    /*
     *  We are in the coordinator context and must wait for the child context to
     *  die.
     */
    int status = 0;
    pid_t waitPid = RL_TEMP_FAILURE_RETRY(
        ::waitpid(_childPID, &status, WUNTRACED | WCONTINUED));
    if (waitPid == -1) {
      RL_LOG_ERRNO();
      ASSERT_TRUE(false);
    }
    ASSERT_EQ(WEXITSTATUS(status), EXIT_SUCCESS);
  }
}
