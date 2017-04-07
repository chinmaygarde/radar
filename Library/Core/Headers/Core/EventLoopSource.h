// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_CORE_EVENT_LOOP_SOURCE_
#define RADAR_CORE_EVENT_LOOP_SOURCE_

#include <Core/IOResult.h>
#include <Core/Macros.h>
#include <Core/Mutex.h>
#include <atomic>
#include <chrono>
#include <functional>
#include <utility>

namespace rl {
namespace core {

class WaitSet;

/**
 *  An event loop source interacts with the wait set on the platform to wake the
 *  event loop when specific events occur. The manner in which events are read,
 *  written and also the mode of interaction of the event loop source with the
 *  wait set is fully customizable.
 */
class EventLoopSource {
 public:
  using Handle = intptr_t;

  struct Handles {
    Handle readHandle;
    Handle writeHandle;

    Handles(Handle aReadHandle, Handle aWriteHandle)
        : readHandle(aReadHandle), writeHandle(aWriteHandle){};
  };

  using RWHandlesProvider = std::function<Handles(void)>;
  using RWHandlesCollector = std::function<void(Handles)>;
  using IOHandler = std::function<IOResult(Handle)>;
  using WakeFunction = std::function<void(IOResult)>;
  using WaitSetUpdateHandler = std::function<void(EventLoopSource& source,
                                                  WaitSet& waitset,
                                                  Handle readHandle,
                                                  bool adding)>;

  /**
   *  Create a custom event loop source with callbacks to specify each detail of
   *  operation of the source.
   */
  EventLoopSource(RWHandlesProvider handleProvider,
                  RWHandlesCollector handleCollector,
                  IOHandler readHandler,
                  IOHandler writeHandler,
                  WaitSetUpdateHandler waitsetUpdateHandler);

  virtual ~EventLoopSource();

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

  /**
   *  The raw IO handles this event loop source uses for reading and writing
   *  data
   *
   *  @return the raw IO handle
   */
  Handles handles();

  void setHandlesProvider(RWHandlesProvider provider);

  void setHandlesCollector(RWHandlesCollector collector);

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

  void updateInWaitSet(WaitSet& waitset, bool shouldAdd);

  void setCustomWaitSetUpdateHandler(WaitSetUpdateHandler updateHandler);

  void attemptRead();

  /**
   *  Create a repeating timer source that fires at the given interval
   *
   *  @param repeatInterval the repeat interval
   *
   *  @return the timer source
   */
  static std::shared_ptr<EventLoopSource> Timer(
      ClockDurationNano repeatInterval);

  /**
   *  Create a trivial loop source
   *
   *  @return the trivial source
   */
  static std::shared_ptr<EventLoopSource> Trivial();

 protected:
  EventLoopSource();

 private:
  RWHandlesProvider _handlesProvider;
  RWHandlesCollector _handlesCollector;
  Handles _handles;
  IOHandler _readHandler;
  IOHandler _writeHandler;
  WaitSetUpdateHandler _customWaitSetUpdateHandler;
  WakeFunction _wakeFunction;
  std::atomic_bool _handlesAllocated;
  Mutex _handlesAllocationMutex;

  void updateInWaitSetForSimpleRead(WaitSet& waitset, bool shouldAdd);

  RL_DISALLOW_COPY_AND_ASSIGN(EventLoopSource);
};

}  // namespace core
}  // namespace rl

#endif  // RADAR_CORE_EVENT_LOOP_SOURCE_
