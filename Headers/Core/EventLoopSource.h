// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_CORE_EVENT_LOOP_SOURCE__
#define __RADARLOVE_CORE_EVENT_LOOP_SOURCE__

#include <Core/Macros.h>
#include <Core/WaitSet.h>

#include <functional>
#include <utility>
#include <chrono>

namespace rl {

/**
 *  An event loop source interacts with the wait set on the platform to wake the
 *  event loop when specific events occur. The manner in which events are read,
 *  written and also the mode of interaction of the event loop source with the
 *  wait set is fully customizable.
 */
class EventLoopSource {
 public:
  using Handle = int;
  using Handles = std::pair<Handle, Handle>;
  using IOHandler = std::function<void(Handle)>;
  using WakeFunction = std::function<void(void)>;
  using RWHandlesProvider = std::function<Handles(void)>;
  using RWHandlesCollector = std::function<void(Handles)>;
  using WaitSetUpdateHandler = std::function<void(EventLoopSource* source,
                                                  WaitSet::Handle waitsetHandle,
                                                  Handle readHandle,
                                                  bool adding)>;

  /**
   *  Create a custom event loop source with callbacks to specify each detail of
   *  operation of the source.
   */
  explicit EventLoopSource(RWHandlesProvider handleProvider,
                           RWHandlesCollector handleCollector,
                           IOHandler readHandler,
                           IOHandler writeHandler,
                           WaitSetUpdateHandler waitsetUpdateHandler);

  ~EventLoopSource();

  void onAwoken();

#pragma mark - Accessing the wait function

  /**
   *  Returns the function that is invoked when the event loop wakes up when
   *  this event loop source is signalled
   *
   *  @return the wake function
   */
  WakeFunction wakeFunction() const;

  /**
   *  Set the function to invoke when the event loop wakes up when this event
   *  loop source is signalled
   *
   *  @param wakeFunction the wake function
   */
  void setWakeFunction(WakeFunction wakeFunction);

#pragma mark - Accessing the underlying handles

  /**
   *  The raw IO handles this event loop source uses for reading and writing
   *  data
   *
   *  @return the raw IO handle
   */
  Handles handles();

  /**
   *  @return the raw read handle
   */
  Handle readHandle();

  /**
   *  @return the raw write handle
   */
  Handle writeHandle();

#pragma mark - Accessing the reader and writer procs

  /**
   *  @return the function that is invoked to read data from the raw read handle
   *          when the wait set detects that there is data to be read
   */
  IOHandler reader() const;

  /**
   *  Set the reader for this source
   *
   *  @param reader the new reader for this source
   */
  void setReader(const IOHandler& reader);

  /**
   *  @return the function that is invoked to write data on the raw write handle
   */
  IOHandler writer() const;

  /**
   *  Set the writer for this source
   *
   *  @param writer the new writer for this source
   */
  void setWriter(const IOHandler& writer);

#pragma mark - Interacting with a WaitSet

  /**
   *  The function that in invoked to modify the raw event loop source handle
   *  on the platform wait set
   *
   *  @param handle    the wait set handle
   *  @param shouldAdd if this source is being added or removed from the wait
   *                   set
   */
  void updateInWaitSetHandle(WaitSet::Handle handle, bool shouldAdd);

#pragma mark - Common event loop sources

  /**
   *  Create a repeating timer source that fires at the given interval
   *
   *  @param repeatInterval the repeat interval
   *
   *  @return the timer source
   */
  static std::shared_ptr<EventLoopSource> Timer(
      std::chrono::nanoseconds repeatInterval);

  /**
   *  Create a trivial loop source
   *
   *  @return the trivial source
   */
  static std::shared_ptr<EventLoopSource> Trivial();

 private:
  RWHandlesProvider _handlesProvider;
  RWHandlesCollector _handlesCollector;
  Handles _handles;
  IOHandler _readHandler;
  IOHandler _writeHandler;
  WaitSetUpdateHandler _customWaitSetUpdateHandler;
  WakeFunction _wakeFunction;
  bool _handlesAllocated;

  /**
   *  If this event loop source can be waitied on for simple reads by the
   *  platform waitset, no custom wait set update handler is needed and this
   *  function is invoked to add the handle for waiting on a read.
   *
   *  @param handle    the wait set handle
   *  @param shouldAdd if this source is being added or removed from the wait
   *                   set
   */
  void updateInWaitSetHandleForSimpleRead(WaitSet::Handle handle,
                                          bool shouldAdd);

  DISALLOW_COPY_AND_ASSIGN(EventLoopSource);
};
}

#endif /* defined(__RADARLOVE_CORE_EVENT_LOOP_SOURCE__) */
