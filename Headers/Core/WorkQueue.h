// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_CORE_WORKQUEUE_H_
#define RADARLOVE_CORE_WORKQUEUE_H_

#include <Core/EventLoop.h>
#include <Core/Latch.h>

#include <thread>
#include <mutex>

namespace rl {
namespace core {

class WorkQueue {
 public:
  explicit WorkQueue(std::string name);

  size_t workerCount() const;

  using WorkItem = std::function<void(void)>;
  void dispatch(WorkItem work);

  ~WorkQueue();

 private:
  std::mutex _lock;
  std::shared_ptr<EventLoopSource> _workSource;
  std::list<std::pair<std::thread, std::shared_ptr<EventLoopSource>>> _workers;
  std::list<WorkItem> _workItems;
  bool _shuttingDown;
  std::condition_variable _idleness;

  void workerMain(std::string name,
                  Latch& latch,
                  std::shared_ptr<EventLoopSource> terminationSource);
  void workerTerminate();
  void work();
  WorkItem acquireWork();
  void waitForAllPendingWorkFlushed();
  bool areWorkItemsPending();
  bool shouldReadWorkSourceHandle();

  void shutdown();

  RL_DISALLOW_COPY_AND_ASSIGN(WorkQueue);
};

}  // namespace core
}  // namespace rl

#endif  // RADARLOVE_CORE_WORKQUEUE_H_
