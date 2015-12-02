// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/WorkQueue.h>

#include <string>

namespace rl {
namespace core {

WorkQueue::WorkQueue(std::string queueName)
    : _workSource(EventLoopSource::Trivial()) {
  /*
   *  Set source handlers
   */
  _workSource->setWakeFunction(std::bind(&WorkQueue::work, this));

  /*
   *  Start worker threads
   */
  const auto poolSize = std::thread::hardware_concurrency();

  Latch ready(poolSize);

  for (auto i = 0; i < poolSize; i++) {
    /*
     *  Each worker thread gets an event loop source whose sole purpose is to
     *  service a loop termination
     */
    auto terminationSource = EventLoopSource::Trivial();
    terminationSource->setWakeFunction(
        std::bind(&WorkQueue::workerTerminate, this));

    std::string name = "rl." + queueName + " .worker" + std::to_string(i + 1);

    std::thread worker(std::bind(&WorkQueue::workerMain, this, name,
                                 std::ref(ready), terminationSource));

    _workers.push_back({std::move(worker), terminationSource});
  }

  /*
   *  Ensure all worker threads are spun up and ready
   */
  ready.wait();
}

void WorkQueue::workerMain(std::string name,
                           Latch& latch,
                           std::shared_ptr<EventLoopSource> terminationSource) {
  thread::SetName(name.c_str());

  auto loop = EventLoop::Current();

  auto result = loop->addSource(_workSource);
  RL_ASSERT(result);

  result = loop->addSource(terminationSource);
  RL_ASSERT(result);

  loop->loop([&latch] { latch.countDown(); });
}

void WorkQueue::workerTerminate() {
  EventLoop::Current()->terminate();
}

void WorkQueue::work() {
  auto item = acquireWork();

  if (item) {
    item();
  }
}

WorkQueue::WorkItem WorkQueue::acquireWork() {
  /*
   *  Only hold the lock long enough to dequeue the work item. Holding the
   *  lock is not necessary while actually performing the work.
   */
  std::lock_guard<std::mutex> lock(_lock);

  if (_workItems.size() == 0) {
    return nullptr;
  }

  auto item = _workItems.front();
  _workItems.pop_front();
  return item;
}

size_t WorkQueue::workerCount() const {
  return _workers.size();
}

void WorkQueue::dispatch(WorkItem item) {
  if (item == nullptr) {
    return;
  }

  /*
   *  Enqueue pending work
   */
  std::lock_guard<std::mutex> lock(_lock);
  _workItems.push_back(item);

  /*
   *  Perform a trivial write on the work source. A waiting worker will wake
   *  to service this
   */
  _workSource->writer()(_workSource->writeHandle());
}

void WorkQueue::shutdown() {
  std::lock_guard<std::mutex> lock(_lock);

  if (_workers.size() == 0) {
    return;
  }

  /*
   *  Trigger a trivial write on the termination source for all threads
   */
  for (auto& worker : _workers) {
    auto source = worker.second;
    source->writer()(source->writeHandle());
  }

  /*
   *  Wait for threads to die :(
   */
  for (auto& worker : _workers) {
    worker.first.join();
  }

  _workers.clear();

  RL_ASSERT(_workers.size() == 0);
}

WorkQueue::~WorkQueue() {
  shutdown();
}

}  // namespace core
}  // namespace rl
