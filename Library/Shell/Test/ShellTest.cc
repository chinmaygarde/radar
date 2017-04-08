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
  auto shell = rl::shell::Shell::CreateWithCurrentThreadAsHost(
      std::make_shared<NullRenderSurface>());
  ASSERT_NE(shell, nullptr);
  shell->shutdown();
}
