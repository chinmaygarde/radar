// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_CORE_SOCKETBOOTSTRAPSERVER_H_
#define RADARLOVE_CORE_SOCKETBOOTSTRAPSERVER_H_

#include <Core/Config.h>

#if RL_CHANNELS == RL_CHANNELS_SOCKET

#include <Core/Macros.h>
#include <Core/EventLoopSource.h>
#include <Core/URI.h>

#include "SocketServer.h"
#include "BootstrapServerProvider.h"

#include <map>

namespace rl {
namespace core {

class SocketBootstrapServerProvider : public BootstrapServerProvider {
 public:
  SocketBootstrapServerProvider(const URI& socketURI);

  ~SocketBootstrapServerProvider();

  std::shared_ptr<EventLoopSource> source();

 private:
  SocketServer _server;
  std::map<std::string, std::shared_ptr<SocketPair>> _registrations;

  void onAccept(SocketPair socket);

#if 0
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
#endif

  RL_DISALLOW_COPY_AND_ASSIGN(SocketBootstrapServer);
};

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_SOCKET

#endif  // RADARLOVE_CORE_SOCKETBOOTSTRAPSERVER_H_
