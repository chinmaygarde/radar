// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_CORE_SOCKETSERVER_H_
#define RADAR_CORE_SOCKETSERVER_H_

#include <Core/Config.h>

#if RL_CHANNELS == RL_CHANNELS_SOCKET

#include <Core/Macros.h>
#include <Core/EventLoopSource.h>

#include "SocketPair.h"

#include <string>

namespace rl {
namespace core {

class SocketServer {
 public:
  using AcceptCallback = std::function<void(SocketPair)>;

  SocketServer(const std::string& name, AcceptCallback callback);

  ~SocketServer();

  bool isValid() const;

  bool listen(size_t backlog) const;

  std::shared_ptr<EventLoopSource> source();

 private:
  SocketPair::Handle _handle;
  bool _isValid;
  std::shared_ptr<EventLoopSource> _source;
  AcceptCallback _acceptCallback;

  RawAttachment accept() const;

  void setupAcceptSource();

  RL_DISALLOW_COPY_AND_ASSIGN(SocketServer);
};

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_SOCKET

#endif  // RADAR_CORE_SOCKETSERVER_H_
