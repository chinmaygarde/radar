/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <TestRunner/TestRunner.h>

#include "Process.h"

namespace rl {
namespace core {
namespace testing {

TEST(ProcessTest, CanLaunchChildProcess) {
  Process process;
  bool didLaunch = false;
  switch (process.run()) {
    case Process::RunResult::Failure:
      ASSERT_TRUE(false);
      break;
    case Process::RunResult::Parent:
      didLaunch = true;
      break;
    case Process::RunResult::Child:
      exit(0);
      break;
  }

  ASSERT_TRUE(didLaunch);
}

}  // namespace testing
}  // namespace core
}  // namespace rl
