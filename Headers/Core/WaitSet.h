// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE__WAITSET__
#define __RADARLOVE__WAITSET__

#include <Core/Macros.h>

#include <set>

namespace rl {
class LooperSource;
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
  WaitSet();

  ~WaitSet();

#pragma mark - Managing wait set sources
  /**
   *  Add a looper source to the wait set
   *
   *  @param source the looper source reference to add
   *
   *  @return if the source was successfully added to the waitset
   */
  bool addSource(std::shared_ptr<LooperSource> source);

  /**
   *  Remove a looper source from the wait set
   *
   *  @param source the looper source reference to remove
   *
   *  @return if the source was successfully removed from the waitset
   */
  bool removeSource(std::shared_ptr<LooperSource> source);

#pragma mark - Waiting on the waitset

  /**
   *  Waits for events to be signalled on the waitset
   *
   *  @return the first looper source signalled on the waitset
   */
  LooperSource* wait();

 private:
  Handle _handle;
  static Handle platformHandleCreate();
  static LooperSource* platformHandleWait(Handle handle);
  static void platformHandleDestory(Handle handle);
  std::set<std::shared_ptr<LooperSource>> _sources;

  DISALLOW_COPY_AND_ASSIGN(WaitSet);
};
}

#endif /* defined(__RADARLOVE__WAITSET__) */
