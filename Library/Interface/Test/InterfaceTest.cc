/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Coordinator/RenderSurface.h>
#include <Interface/Interface.h>
#include <Shell/Shell.h>
#include <TestRunner/InterfaceTest.h>
#include <TestRunner/TestRunner.h>

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
  auto interface = std::make_unique<rl::interface::Interface>(delegate);

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
