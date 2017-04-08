/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <thread>

namespace rl {
namespace core {

#ifndef NDEBUG
#define RL_DEBUG_THREAD_GUARD(x) rl::core::ThreadGuard x
#else  // NDEBUG
#define RL_DEBUG_THREAD_GUARD(x)
#endif  // NDEBUG

class ThreadGuard {
 public:
  ThreadGuard() : _threadID(std::this_thread::get_id()) {}

  ThreadGuard(ThreadGuard&& other) { _threadID = other._threadID; }

  ~ThreadGuard() { RL_ASSERT(isThreadValid()); }

  bool isThreadValid() const { return std::this_thread::get_id() == _threadID; }

 private:
  std::thread::id _threadID;

  RL_DISALLOW_COPY_AND_ASSIGN(ThreadGuard);
};

}  // namespace core
}  // namespace rl
