// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_CORE_WORKQUEUE_H_
#define RADAR_CORE_WORKQUEUE_H_

#include <Core/EventLoop.h>
#include <Core/EventLoopThread.h>
#include <Core/Latch.h>

namespace rl {
namespace core {

class WorkQueue {
 public:
  using WorkItem = std::function<void(void)>;

  WorkQueue();

  ~WorkQueue();

  RL_WARN_UNUSED_RESULT
  bool dispatch(WorkItem work);

  size_t workerCount() const;

 private:
  std::shared_ptr<EventLoopSource> _workSource;
  std::vector<std::unique_ptr<EventLoopThread>> _workers;
  Mutex _workItemsMutex;
  std::list<WorkItem> _workItems RL_GUARDED_BY(_workItemsMutex);

  void work();
  WorkItem acquireWork();

  RL_DISALLOW_COPY_AND_ASSIGN(WorkQueue);
};

}  // namespace core
}  // namespace rl

#endif  // RADAR_CORE_WORKQUEUE_H_
