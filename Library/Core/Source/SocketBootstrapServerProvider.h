// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_CORE_SOCKETBOOTSTRAPSERVER_H_
#define RADARLOVE_CORE_SOCKETBOOTSTRAPSERVER_H_

#include "Config.h"

#if RL_CHANNELS == RL_CHANNELS_SOCKET

#include <Core/Macros.h>
#include <Core/EventLoopThread.h>

#include "SocketServer.h"
#include "BootstrapServerProvider.h"

#include <map>

namespace rl {
namespace core {

class SocketBootstrapServerProvider : public BootstrapServerProvider {
 public:
  static const char* const kDefaultSocketPath;

  SocketBootstrapServerProvider();

  ~SocketBootstrapServerProvider();

 private:
  SocketServer _server;
  core::EventLoopThread _thread;
  std::map<std::string, std::shared_ptr<SocketPair>> _registrations;

  void serverMain(Latch& latch);

  void onAccept(RawAttachment socket);

  RL_DISALLOW_COPY_AND_ASSIGN(SocketBootstrapServerProvider);
};

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_SOCKET

#endif  // RADARLOVE_CORE_SOCKETBOOTSTRAPSERVER_H_
