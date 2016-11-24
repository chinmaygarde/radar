// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_CORE_THREADGUARD_H_
#define RADAR_CORE_THREADGUARD_H_

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

#endif  // RADAR_CORE_THREADGUARD_H_
