/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include "Config.h"

#if RL_CHANNELS == RL_CHANNELS_SOCKET

#include <Core/Allocation.h>
#include <Core/Channel.h>
#include <Core/ChannelProvider.h>
#include <Core/RawAttachment.h>
#include <mutex>
#include "SocketPair.h"

namespace rl {
namespace core {

class SocketChannel : public ChannelProvider {
 public:
  SocketChannel(Channel& owner);

  SocketChannel(Channel& owner, RawAttachment attachment);

  ~SocketChannel();

  std::shared_ptr<EventLoopSource> createSource() const override;

  IOResult writeMessages(Messages&& message,
                         ClockDurationNano timeout) override;

  IOReadResult readMessage(ClockDurationNano timeout) override;

  AttachmentRef attachment() override;

  bool doTerminate() override;

 private:
  Channel& _channel;
  std::shared_ptr<SocketPair> _pair;

  std::mutex _readBufferMutex;
  Allocation _inlineMessageBuffer;
  Allocation _controlBuffer;

  RL_WARN_UNUSED_RESULT
  bool setup();

  IOResult writeMessageSingle(const Message& message,
                              ClockDurationNano timeout);

  RL_DISALLOW_COPY_AND_ASSIGN(SocketChannel);
};

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_SOCKET
