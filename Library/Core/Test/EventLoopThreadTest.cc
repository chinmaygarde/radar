/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/EventLoopThread.h>
#include <TestRunner/TestRunner.h>

namespace rl {
namespace core {
namespace testing {

TEST(NamespaceTest, SimpleInitShutdown) {
  EventLoopThread thread;
  ASSERT_TRUE(thread.loop().get() != nullptr);
}

}  // namespace testing
}  // namespace core
}  // namespace rl
