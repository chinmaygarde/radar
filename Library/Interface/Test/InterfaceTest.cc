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

  rl::shell::Shell& currentShell() {
    RL_ASSERT(_shell != nullptr);
    return *_shell;
  }

 private:
  std::shared_ptr<NullRenderSurface> _renderSurface;
  std::shared_ptr<rl::shell::Shell> _shell;

  RL_DISALLOW_COPY_AND_ASSIGN(InterfaceTest);
};

TEST_F(InterfaceTest, SimpleFixtureInitialization) {
  /*
   *  Just testing the fixture setup.
   */
  ASSERT_TRUE(true);
}

static std::shared_ptr<rl::interface::DefaultInterfaceDelegate>
CreateRegisteredInterface(
    rl::shell::Shell& shell,
    std::shared_ptr<rl::interface::DefaultInterfaceDelegate> delegate =
        std::make_shared<rl::interface::DefaultInterfaceDelegate>()) {
  auto interface = rl::core::make_unique<rl::interface::Interface>(delegate);

  if (shell.registerManagedInterface(std::move(interface))) {
    return delegate;
  }

  return nullptr;
}

static void RunInActivatedInterface(
    rl::shell::Shell& shell,
    rl::interface::DefaultInterfaceDelegate::InterfaceCallback callback) {
  if (callback == nullptr) {
    return;
  }

  auto interface = std::make_shared<rl::interface::DefaultInterfaceDelegate>();

  interface->setInterfaceDidBecomeActive(callback);

  ASSERT_NE(CreateRegisteredInterface(shell, interface), nullptr);
}

TEST_F(InterfaceTest, AddManagedInterface) {
  ASSERT_NE(CreateRegisteredInterface(currentShell()), nullptr);
}

TEST_F(InterfaceTest, TestBecomesActive) {
  rl::core::Latch latch(1);

  auto interface = std::make_shared<rl::interface::DefaultInterfaceDelegate>();

  bool active = false;

  interface->setInterfaceDidBecomeActive(
      [&](rl::interface::Interface& interface) {
        active = true;
        latch.countDown();
      });

  interface = CreateRegisteredInterface(currentShell(), interface);

  ASSERT_NE(interface, nullptr);

  latch.wait();
  ASSERT_TRUE(active);
}

TEST_F(InterfaceTest, TestActivatedInterface) {
  rl::core::Latch latch(1);
  bool active = false;
  RunInActivatedInterface(currentShell(),
                          [&](rl::interface::Interface& interface) {
                            active = true;
                            latch.countDown();
                          });

  latch.wait();
  ASSERT_TRUE(active);
}
