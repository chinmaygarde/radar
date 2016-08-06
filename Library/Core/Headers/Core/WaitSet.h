// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_CORE_WAITSET_
#define RADARLOVE_CORE_WAITSET_

#include <Core/Macros.h>
#include <Core/EventLoopSource.h>
#include <Core/Mutexed.h>

#include <unordered_set>

namespace rl {
namespace core {

class WaitSetProvider;
class EventLoopSource;
/**
 *  A waitset is an abstraction over a platform specific event multiplexing API
 */
class WaitSet {
 public:
  using Handle = uintptr_t;

  enum class WakeReason {
    ReadAvailable,
    Timeout,
    Error,
  };

  using Result = std::pair<WakeReason, EventLoopSource*>;

  /**
   *  Create an empty wait set to which sources can be added
   */
  WaitSet();

  ~WaitSet();

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

  /**
   *  Waits for events to be signalled on the waitset
   *
   *  @return the result of the wait
   */
  Result wait(ClockDurationNano timeout);

  Handle handle() const;

  WaitSetProvider& provider() const;

 private:
  using EventLoopSourceRef = std::shared_ptr<EventLoopSource>;

  std::unique_ptr<WaitSetProvider> _provider;
  Mutexed<std::unordered_set<EventLoopSourceRef>> _sources;

  RL_DISALLOW_COPY_AND_ASSIGN(WaitSet);
};

}  // namespace core
}  // namespace rl

#endif  // RADARLOVE_CORE_WAITSET_
