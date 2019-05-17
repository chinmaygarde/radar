/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <thread>

#include <Core/Thread.h>
#include <TestRunner/TestRunner.h>

namespace rl {
namespace core {
namespace testing {

#if !RL_OS_BSD && !RL_OS_WINDOWS && !RL_OS_NACL

TEST(ThreadTest, GetSetName) {
  std::thread thread([&] {
    const char name[] = "DeRp";
    thread::SetName(name);
    ASSERT_EQ(::strncmp(name, thread::GetName().c_str(), sizeof(name)), 0);
  });
  thread.join();
}

#endif  // !RL_OS_BSD && !RL_OS_WINDOWS && !RL_OS_NACL

}  // namespace testing
}  // namespace core
}  // namespace rl
