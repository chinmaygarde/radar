/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Shell/Shell.h>
#include <TestRunner/TestRunner.h>
#include <sys/wait.h>
#include <unistd.h>
#include "NullRenderSurface.h"
#include "ShellXPCTest.h"

TEST(ShellTest, SimpleInitialization) {
  std::thread thread([]() {
    auto shell = rl::shell::Shell::CreateWithCurrentThreadAsHost(
        std::make_shared<rl::shell::NullRenderSurface>());
    ASSERT_NE(shell, nullptr);
    shell->shutdown();
  });
  thread.join();
}

TEST(ShellTest, InitializeMutlipleShells) {
  std::thread thread1([]() {
    auto shell1 = rl::shell::Shell::CreateWithCurrentThreadAsHost(
        std::make_shared<rl::shell::NullRenderSurface>());
    ASSERT_NE(shell1, nullptr);
    rl::core ::Latch latch(1);
    std::thread thread2([&latch]() {
      auto shell2 = rl::shell::Shell::CreateWithCurrentThreadAsHost(
          std::make_shared<rl::shell::NullRenderSurface>());
      ASSERT_NE(shell2, nullptr);
      latch.countDown();
      shell2->shutdown();
    });
    latch.wait();
    shell1->shutdown();
    thread2.join();
  });
  thread1.join();
}

TEST_F(ShellXPCTest, FixtureRuns) {
  ASSERT_TRUE(true);
}
