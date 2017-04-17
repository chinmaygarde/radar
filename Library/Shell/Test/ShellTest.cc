/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <TestRunner/TestRunner.h>

#include <Shell/Shell.h>

class NullRenderSurface : public rl::coordinator::RenderSurface {
 public:
  NullRenderSurface(){};

  bool makeCurrent() override { return false; }

  bool present() override { return false; }

  void accessWillBegin() override {}

  void accessDidEnd() override {}

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(NullRenderSurface);
};

TEST(ShellTest, SimpleInitialization) {
  std::thread thread([]() {
    auto shell = rl::shell::Shell::CreateWithCurrentThreadAsHost(
        std::make_shared<NullRenderSurface>());
    ASSERT_NE(shell, nullptr);
    shell->shutdown();
  });
  thread.join();
}

TEST(ShellTest, InitializeMutlipleShells) {
  std::thread thread1([]() {
    auto shell1 = rl::shell::Shell::CreateWithCurrentThreadAsHost(
        std::make_shared<NullRenderSurface>());
    ASSERT_NE(shell1, nullptr);
    rl::core ::Latch latch(1);
    std::thread thread2([&latch]() {
      auto shell2 = rl::shell::Shell::CreateWithCurrentThreadAsHost(
          std::make_shared<NullRenderSurface>());
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
