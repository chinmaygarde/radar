/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/EventLoop.h>
#include <Core/Latch.h>
#include <Core/Macros.h>
#include <Core/Mutexed.h>
#include <thread>

namespace rl {
namespace core {

class EventLoopThread {
 public:
  using MutexedLoop = Mutexed<EventLoop*>;

  EventLoopThread();

  EventLoopThread(Latch& ready);

  ~EventLoopThread();

  MutexedLoop::Access loop();

 private:
  std::unique_ptr<std::thread> _thread;
  MutexedLoop _loop;

  void main(Latch& ready);

  RL_DISALLOW_COPY_AND_ASSIGN(EventLoopThread);
};

}  // namespace core
}  // namespace rl
