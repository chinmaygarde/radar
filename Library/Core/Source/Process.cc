/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "Process.h"

#include <unistd.h>

namespace rl {
namespace core {

Process::Process() = default;

Process::~Process() = default;

Process::RunResult Process::run() const {
  auto result = RL_TEMP_FAILURE_RETRY(::fork());
  if (result == -1) {
    RL_LOG_ERRNO();
    return RunResult::Failure;
  }

  return result > 0 ? Process::RunResult::Parent : Process::RunResult::Child;
}

}  // namespace core
}  // namespace rl
