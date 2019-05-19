/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <TestRunner/TestRunner.h>

#include "Process.h"

namespace rl {
namespace core {

TEST(ProcessTest, CanLaunchChildProcess) {
  Process process;
  switch (process.run()) {
    case Process::RunResult::Failure:
      ASSERT_TRUE(false);
      break;
    case Process::RunResult::Parent:
      break;
    case Process::RunResult::Child:
      exit(0);
      break;
  }
}

}  // namespace core
}  // namespace rl
