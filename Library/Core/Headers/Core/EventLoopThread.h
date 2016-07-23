// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_CORE_EVENTLOOPTHREAD_H_
#define RADAR_CORE_EVENTLOOPTHREAD_H_

#include <Core/Macros.h>
#include <Core/EventLoop.h>
#include <Core/Mutexed.h>
#include <Core/Latch.h>

#include <thread>

namespace rl {
namespace core {

class EventLoopThread {
 public:
  EventLoopThread();

  ~EventLoopThread();

  using MutexedLoop = Mutexed<EventLoop*>;

  MutexedLoop::Access loop();

 private:
  std::unique_ptr<std::thread> _thread;
  MutexedLoop _loop;

  void main(Latch& ready);

  void terminate();

  RL_DISALLOW_COPY_AND_ASSIGN(EventLoopThread);
};

}  // namespace core
}  // namespace rl

#endif  // RADAR_CORE_EVENTLOOPTHREAD_H_
