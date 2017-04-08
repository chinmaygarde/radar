/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/EventLoopThread.h>
#include <TestRunner/TestRunner.h>

TEST(NamespaceTest, SimpleInitShutdown) {
  rl::core::EventLoopThread thread;
  ASSERT_TRUE(thread.loop().get() != nullptr);
}
