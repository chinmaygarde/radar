// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_CORE_CHANNELPROVIDER__
#define __RADARLOVE_CORE_CHANNELPROVIDER__

#include <Core/EventLoopSource.h>
#include <Core/Message.h>

#include <vector>
#include <memory>

namespace rl {

class ChannelProvider {
 public:
  enum Result {
    Success = 0,
    TemporaryFailure,
    PermanentFailure,
  };

  using ReadResult = std::pair<Result, Messages>;

  /**
   *  Get a loop source for this channel provider
   *
   *  @return the loop source for this channel provider
   */
  virtual std::shared_ptr<EventLoopSource> createSource() const = 0;

  /**
   *  Write messages on the channel provider
   *
   *  @param messages the messages to write
   *
   *  @return the write result
   */
  virtual Result WriteMessages(Messages&& message,
                               ClockDurationNano timeout) = 0;

  /**
   *  Read a message on the channel provider
   *
   *  @return the read result
   */
  virtual ReadResult ReadMessages(ClockDurationNano timeout) = 0;

  /**
   *  Perform the actual channel connection termination
   *
   *  @return if the termination was successful
   */
  virtual bool doTerminate() = 0;

  virtual ~ChannelProvider() {}
};
}  // namespace rl

#endif /* defined(__RADARLOVE_CORE_CHANNELPROVIDER__) */
