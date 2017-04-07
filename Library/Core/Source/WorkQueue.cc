// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Thread.h>
#include <Core/WorkQueue.h>
#include <algorithm>

namespace rl {
namespace core {

WorkQueue::WorkQueue() : _workSource(EventLoopSource::Trivial()) {
  /*
   *  Start worker threads
   */
  const auto poolSize =
      std::min<uint32_t>(std::thread::hardware_concurrency(), 8);

  Latch ready(poolSize);

  for (size_t i = 0; i < poolSize; i++) {
    _workers.emplace_back(std::make_unique<EventLoopThread>(ready));
  }

  ready.wait();

  /*
   *  Setup work source and add that source to all workers.
   */
  _workSource->setWakeFunction(std::bind(&WorkQueue::work, this));

  for (size_t i = 0; i < poolSize; i++) {
    auto loopAccess = _workers[i]->loop();
    EventLoop* loop = loopAccess.get();
    loop->addSource(_workSource);
  }
}

WorkQueue::~WorkQueue() {
  _workers.clear();
}

void WorkQueue::work() {
  while (auto item = acquireWork()) {
    item();
  }
}

WorkQueue::WorkItem WorkQueue::acquireWork() {
  /*
   *  Only hold the lock long enough to dequeue the work item. Holding the
   *  lock is not necessary while actually performing the work.
   */
  MutexLocker lock(_workItemsMutex);
  auto workItemsCount = _workItems.size();
  if (workItemsCount == 0) {
    /*
     *  No more work to do.
     */
    return nullptr;
  }

  /*
   *  There are one or more work items in the queue.
   */
  auto item = _workItems.front();
  _workItems.pop_front();
  return item;
}

size_t WorkQueue::workerCount() const {
  return _workers.size();
}

bool WorkQueue::dispatch(WorkItem work) {
  if (work == nullptr) {
    return false;
  }

  auto initialCount = 1;

  {
    /*
     *  Enqueue pending work. Don't hold to lock for the write to the trivial
     *  source.
     */
    MutexLocker lock(_workItemsMutex);
    initialCount = _workItems.size();
    _workItems.push_back(work);
  }

  if (initialCount == 0) {
    /*
     *  Perform a trivial write on the work source. A waiting worker will wake
     *  to service this.
     */
    _workSource->writer()(_workSource->handles().writeHandle);
  }

  return true;
}

}  // namespace core
}  // namespace rl
