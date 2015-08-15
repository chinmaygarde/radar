// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE__LOOPERSOURCE__
#define __RADARLOVE__LOOPERSOURCE__

#include <Core/Macros.h>
#include <Core/WaitSet.h>

#include <functional>
#include <utility>
#include <chrono>

namespace rl {

class LooperSource {
 public:
  using Handle = int;
  using Handles = std::pair<Handle, Handle>;
  using IOHandler = std::function<void(Handle)>;
  using WakeFunction = std::function<void(void)>;
  using RWHandlesProvider = std::function<Handles(void)>;
  using RWHandlesCollector = std::function<void(Handles)>;
  using WaitSetUpdateHandler = std::function<void(LooperSource* source,
                                                  WaitSet::Handle waitsetHandle,
                                                  Handle readHandle,
                                                  bool adding)>;

  LooperSource(RWHandlesProvider handleProvider,
               RWHandlesCollector handleCollector,
               IOHandler readHandler,
               IOHandler writeHandler,
               WaitSetUpdateHandler waitsetUpdateHandler);

  ~LooperSource();

  void onAwoken();

#pragma mark - Accessing the wait function

  WakeFunction wakeFunction() const;

  void setWakeFunction(WakeFunction wakeFunction);

#pragma mark - Accessing the underlying handles

  Handles handles();

  Handle readHandle();

  Handle writeHandle();

#pragma mark - Accessing the reader and writer procs

  IOHandler reader();

  IOHandler writer();

#pragma mark - Interacting with a WaitSet

  void updateInWaitSetHandle(WaitSet::Handle handle, bool shouldAdd);

#pragma mark - Common Looper Sources

  /**
   *  Create a repeating timer source that fires at the given interval
   *
   *  @param repeatInterval the repeat interval
   *
   *  @return the timer source
   */
  static std::shared_ptr<LooperSource> AsTimer(
      std::chrono::nanoseconds repeatInterval);

  /**
   *  Create a trivial looper source
   *
   *  @return the trivial source
   */
  static std::shared_ptr<LooperSource> AsTrivial();

 private:
  RWHandlesProvider _handlesProvider;
  RWHandlesCollector _handlesCollector;
  Handles _handles;
  IOHandler _readHandler;
  IOHandler _writeHandler;
  WaitSetUpdateHandler _customWaitSetUpdateHandler;
  WakeFunction _wakeFunction;
  bool _handlesAllocated;

  DISALLOW_COPY_AND_ASSIGN(LooperSource);
};
}

#endif /* defined(__RADARLOVE__LOOPERSOURCE__) */
