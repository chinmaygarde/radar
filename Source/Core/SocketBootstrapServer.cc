// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if RL_CHANNELS == RL_CHANNELS_SOCKET

#include <Core/BootstrapServer.h>
#include <Core/SocketBootstrapServer.h>
#include <Core/SocketChannel.h>

namespace rl {
namespace core {

SocketBootstrapServer::SocketBootstrapServer() {
  EventLoopSource::RWHandlesProvider provider = []() {
    return EventLoopSource::Handles(
        SocketChannel::CreateServerHandle("/tmp/radar_bootstrap.sk"), 0);
  };

  EventLoopSource::RWHandlesCollector collector = [](
      EventLoopSource::Handles handles) {
    auto closed = SocketChannel::DestroyHandle(
        static_cast<SocketChannel::Handle>(handles.first));
    RL_ASSERT(closed);
    RL_ASSERT(handles.second == 0);
  };

  EventLoopSource::IOHandler listenReadHandler = std::bind(
      &SocketBootstrapServer::onListenReadResult, this, std::placeholders::_1);

  _source = std::make_shared<EventLoopSource>(
      provider, collector, listenReadHandler, nullptr, nullptr);
}

SocketBootstrapServer::~SocketBootstrapServer() {
  _source->setReader(nullptr);
}

std::shared_ptr<EventLoopSource> SocketBootstrapServer::source() const {
  return _source;
}

IOResult SocketBootstrapServer::onListenReadResult(
    EventLoopSource::Handle readHandle) {
  RL_ASSERT_MSG(false, "WIP");
  return IOResult::Timeout;
}

bool BootstrapServerAdvertise(const std::string&,
                              std::shared_ptr<core::Channel>) {
  RL_ASSERT_MSG(false, "WIP");
  return false;
}

std::shared_ptr<core::Channel> BootstrapServerAcquireAdvertised(
    const std::string&) {
  RL_ASSERT_MSG(false, "WIP");
  return nullptr;
}

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_SOCKET
