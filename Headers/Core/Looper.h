// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE__LOOPER__
#define __RADARLOVE__LOOPER__

#include "Base.h"
#include "LooperSource.h"
#include "WaitSet.h"
#include "Core/Lock.h"

#include <functional>
#include <list>

namespace rl {

class Looper {
 public:
  using Block = std::function<void(void)>;
  using PendingBlocks = std::list<Block>;

  void loop();

  void terminate();

  static Looper* Current();

  bool addSource(std::shared_ptr<LooperSource> source);
  bool removeSource(std::shared_ptr<LooperSource> source);

  void dispatchAsync(Block block);

 private:
  Looper();
  ~Looper();

  void beforeSleep();
  void afterSleep();

  void flushPendingDispatches();

  WaitSet _waitSet;
  std::shared_ptr<LooperSource> _trivialSource;
  bool _shouldTerminate;
  Lock _lock;
  std::unique_ptr<PendingBlocks> _pendingDispatches;

  DISALLOW_COPY_AND_ASSIGN(Looper);
};
}

#endif /* defined(__RADARLOVE__LOOPER__) */
