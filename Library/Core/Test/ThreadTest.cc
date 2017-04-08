/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/Thread.h>
#include <TestRunner/TestRunner.h>
#include <thread>

#if !RL_OS_BSD && !RL_OS_WINDOWS && !RL_OS_NACL

TEST(ThreadTest, GetSetName) {
  std::thread thread([&] {
    const char name[] = "DeRp";
    rl::core::thread::SetName(name);
    ASSERT_EQ(
        ::strncmp(name, rl::core::thread::GetName().c_str(), sizeof(name)), 0);
  });
  thread.join();
}

#endif  // !RL_OS_BSD && !RL_OS_WINDOWS && !RL_OS_NACL
