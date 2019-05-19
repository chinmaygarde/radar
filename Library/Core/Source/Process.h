/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>

namespace rl {
namespace core {

class Process {
 public:
  Process();

  ~Process();

  enum class RunResult {
    Failure,
    Parent,
    Child,
  };

  RunResult run() const;

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(Process);
};

}  // namespace core
}  // namespace rl
