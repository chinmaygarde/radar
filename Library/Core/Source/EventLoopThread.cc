// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/EventLoopThread.h>

namespace rl {
namespace core {

EventLoopThread::EventLoopThread() : _loop(nullptr) {
  core::Latch ready(1);
  _thread = core::make_unique<std::thread>(
      std::bind(&EventLoopThread::main, this, std::ref(ready)));
  ready.wait();
}

EventLoopThread::EventLoopThread(Latch& ready) : _loop(nullptr) {
  _thread = core::make_unique<std::thread>(
      std::bind(&EventLoopThread::main, this, std::ref(ready)));
}

EventLoopThread::~EventLoopThread() {
  if (_thread == nullptr) {
    return;
  }

  /*
   *  Signal the termination source of the thread.
   */
  {
    auto loopAccess = _loop.access();
    if (EventLoop* loop = loopAccess.get()) {
      loop->terminate();
    }
  }

  /*
   *  Wait for the thread to die naturally.
   */
  _thread->join();
  _thread = nullptr;
}

void EventLoopThread::main(Latch& ready) {
  auto loop = EventLoop::Current();

  {
    /*
     *  Store the reference to the event loop for this thread.
     */
    auto loopAccess = _loop.access();
    loopAccess.get() = loop;
  }

  loop->loop([&ready]() { ready.countDown(); });

  {
    /*
     *  Clear the reference to the event loop for this thread.
     */
    auto loopAccess = _loop.access();
    loopAccess.get() = nullptr;
  }
}

EventLoopThread::MutexedLoop::Access EventLoopThread::loop() {
  return _loop.access();
}

}  // namespace core
}  // namespace rl
