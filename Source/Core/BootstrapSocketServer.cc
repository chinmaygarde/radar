// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/BootstrapSocketServer.h>
#include <Core/SocketChannel.h>

namespace rl {
namespace core {

BootstrapSocketServer::BootstrapSocketServer() {
  EventLoopSource::RWHandlesProvider provider = []() {
    return EventLoopSource::Handles(
        SocketChannel::CreateServerHandle("/tmp/radar_bootstrap.sk"), 0);
  };

  EventLoopSource::RWHandlesCollector collector = [](
      EventLoopSource::Handles handles) {
    auto closed = SocketChannel::DestroyServerHandle(
        static_cast<SocketChannel::Handle>(handles.first));
    RL_ASSERT(closed);
    RL_ASSERT(handles.second == 0);
  };

  EventLoopSource::IOHandler listenReadHandler = std::bind(
      &BootstrapSocketServer::onListenReadResult, this, std::placeholders::_1);

  _source = std::make_shared<EventLoopSource>(
      provider, collector, listenReadHandler, nullptr, nullptr);
}

BootstrapSocketServer::~BootstrapSocketServer() {
  _source->setReader(nullptr);
}

std::shared_ptr<EventLoopSource> BootstrapSocketServer::source() const {
  return _source;
}

IOResult BootstrapSocketServer::onListenReadResult(
    EventLoopSource::Handle readHandle) {
  RL_ASSERT_MSG(false, "WIP");
  return IOResult::Timeout;
}

}  // namespace core
}  // namespace rl
