// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_CORE_CHANNELPROVIDER_
#define RADARLOVE_CORE_CHANNELPROVIDER_

#include <Core/EventLoopSource.h>
#include <Core/IOResult.h>
#include <Core/Message.h>

#include <memory>
#include <vector>

namespace rl {
namespace core {

class ChannelProvider {
 public:
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
  virtual IOResult writeMessages(Messages&& messages,
                                 ClockDurationNano timeout) = 0;

  /**
   *  Read a message on the channel provider
   *
   *  @return the read result
   */
  virtual IOReadResult readMessage(ClockDurationNano timeout) = 0;

  /**
   *  Perform the actual channel connection termination
   *
   *  @return if the termination was successful
   */
  virtual bool doTerminate() = 0;

  /**
   *  Since channels can be sent as message down other channels, providers must
   *  return an attachment appropriate for transport
   *
   *  @return the attachment for this provider
   */
  virtual AttachmentRef attachment() = 0;

  /**
   *  The desctuctor
   */
  virtual ~ChannelProvider() {}
};

}  // namespace core
}  // namespace rl

#endif  // RADARLOVE_CORE_CHANNELPROVIDER_
