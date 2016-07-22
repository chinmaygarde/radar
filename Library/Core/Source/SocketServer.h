// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_CORE_SOCKETSERVER_H_
#define RADAR_CORE_SOCKETSERVER_H_

#include <Core/Config.h>

#if RL_CHANNELS == RL_CHANNELS_SOCKET

#include <Core/Macros.h>

#include "SocketPair.h"

#include <string>

namespace rl {
namespace core {

class SocketServer {
 public:
  SocketServer(const std::string& name);

  ~SocketServer();

  bool listen(size_t backlog) const;

  RawAttachment accept() const;

 private:
  SocketPair::Handle _handle;
  bool _isValid;

  RL_DISALLOW_COPY_AND_ASSIGN(SocketServer);
};

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_SOCKET

#endif  // RADAR_CORE_SOCKETSERVER_H_
