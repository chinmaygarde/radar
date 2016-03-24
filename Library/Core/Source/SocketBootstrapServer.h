// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_CORE_SOCKETBOOTSTRAPSERVER_H_
#define RADARLOVE_CORE_SOCKETBOOTSTRAPSERVER_H_

#include <Core/Config.h>

#if RL_CHANNELS == RL_CHANNELS_SOCKET

#include <Core/Macros.h>
#include <Core/EventLoopSource.h>

#include <map>

namespace rl {
namespace core {

class SocketBootstrapServer {
 public:
  SocketBootstrapServer();

  ~SocketBootstrapServer();

  std::shared_ptr<EventLoopSource> source() const;

 private:
  std::shared_ptr<EventLoopSource> _source;
  std::map<std::string, std::shared_ptr<Channel>> _channels;

  IOResult onListenReadResult(EventLoopSource::Handle readHandle);

  bool attemptRegistration(const std::string& name,
                           Message::Attachment channelAttachment);

  std::shared_ptr<Channel> acquireRegistration(const std::string& name);

  using BootstrapResponse = std::pair<bool, std::shared_ptr<Channel>>;

  static IOResult sendBoostrapResponse(Channel& replyChannel,
                                       BootstrapResponse response);

  BootstrapResponse processBootstrapMessageRequest(Message&& requestMessage);

  IOResult processBootstrapMessageRequestAndReply(Channel& replyChannel,
                                                  Message&& requestMessage);

  RL_DISALLOW_COPY_AND_ASSIGN(SocketBootstrapServer);
};

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_SOCKET

#endif  // RADARLOVE_CORE_SOCKETBOOTSTRAPSERVER_H_
