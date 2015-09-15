// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_CORE_WAITSET__
#define __RADARLOVE_CORE_WAITSET__

#include <Core/Macros.h>

#include <vector>

namespace rl {
class EventLoopSource;
/**
 *  A waitset is an abstraction over a platform specific event multiplexing API
 */
class WaitSet {
 public:
  using Handle = int;

#pragma mark - Creating Wait Sets
  /**
   *  Create an empty wait set to which sources can be added
   */
  explicit WaitSet();

  ~WaitSet();

#pragma mark - Managing wait set sources
  /**
   *  Add a loop source to the wait set
   *
   *  @param source the loop source reference to add
   *
   *  @return if the source was successfully added to the waitset
   */
  bool addSource(std::shared_ptr<EventLoopSource> source);

  /**
   *  Remove a loop source from the wait set
   *
   *  @param source the loop source reference to remove
   *
   *  @return if the source was successfully removed from the waitset
   */
  bool removeSource(std::shared_ptr<EventLoopSource> source);

#pragma mark - Waiting on the waitset

  /**
   *  Waits for events to be signalled on the waitset
   *
   *  @return the first loop source signalled on the waitset
   */
  EventLoopSource& wait();

 private:
  Handle _handle;
  static Handle platformHandleCreate();
  EventLoopSource& platformHandleWait(Handle handle);
  static void platformHandleDestory(Handle handle);
  std::vector<std::shared_ptr<EventLoopSource>> _sources;

  RL_DISALLOW_COPY_AND_ASSIGN(WaitSet);
};
}

#endif /* defined(__RADARLOVE_CORE_WAITSET__) */
