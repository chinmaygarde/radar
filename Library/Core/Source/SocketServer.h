// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include "Config.h"

#if RL_CHANNELS == RL_CHANNELS_SOCKET

#include <Core/Channel.h>
#include <Core/EventLoopSource.h>
#include <Core/Macros.h>
#include <Core/URI.h>
#include <string>
#include "SocketPair.h"

namespace rl {
namespace core {

class SocketServer {
 public:
  using AcceptCallback = std::function<void(RawAttachment)>;

  SocketServer();

  ~SocketServer();

  static std::unique_ptr<Channel> ConnectedChannel(URI uri);

  bool isValid() const;

  bool bind(URI uri, bool unlinkExistingBinding) const;

  bool listen(size_t backlog) const;

  std::shared_ptr<EventLoopSource> acceptSource(AcceptCallback callback) const;

 private:
  SocketPair::Handle _handle;
  bool _isValid;
  std::shared_ptr<EventLoopSource> _acceptSource;

  RawAttachment accept() const;

  bool connect(URI uri) const;

  RL_WARN_UNUSED_RESULT
  RawAttachment takeHandle();

  RL_DISALLOW_COPY_AND_ASSIGN(SocketServer);
};

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_SOCKET
