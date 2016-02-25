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

static const char* SocketBootstrapServerName = "/tmp/radar_bootstrap.sk";

/*
 *  All bootstrap server-client communication operates on this timeout. (100ms)
 */
static const ClockDurationNano SocketBootstrapServerTimeout(100000000);

SocketBootstrapServer::SocketBootstrapServer() {
  EventLoopSource::RWHandlesProvider provider = []() {
    return EventLoopSource::Handles(
        SocketChannel::CreateServerHandle(SocketBootstrapServerName), 0);
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

IOResult BootstrapServerAdvertise(const std::string& name,
                                  std::shared_ptr<Channel> channel) {
  if (name.length() == 0 || !channel) {
    return IOResult::Failure;
  }

  Message::Attachment localChannelAttachment = channel->asMessageAttachment();

  if (!localChannelAttachment.isValid()) {
    return IOResult::Failure;
  }

  auto serverHandle =
      SocketChannel::CreateClientHandle(SocketBootstrapServerName);

  if (serverHandle == -1) {
    /*
     *  Could not connect to the bootstrap server. A later connection may be
     *  successful.
     */
    return IOResult::Timeout;
  }

  Message::Attachment serverAttachment(serverHandle);

  RL_ASSERT(serverAttachment.isValid());

  Channel serverChannel(std::move(serverAttachment));

  Message requestMessage;

  if (!requestMessage.encode(name) ||
      !requestMessage.addAttachment(std::move(localChannelAttachment))) {
    /*
     *  Could not encode the request message.
     */
    return IOResult::Failure;
  }

  bool registrationResult = false;

  Channel::MessageCallback responseCallback = [&registrationResult](
      Message responseMessage, Namespace* ns) {

    if (responseMessage.readCompleted()) {
      return;
    }

    if (!responseMessage.decode(registrationResult, ns)) {
      registrationResult = false;
      return;
    }
  };

  serverChannel.setMessageCallback(responseCallback);

  Messages messages;
  messages.push_back(std::move(requestMessage));

  auto result = serverChannel.sendMessages(std::move(messages));

  if (result != IOResult::Success) {
    /*
     *  Sending the request message must be successful.
     */
    return result;
  }

  result = serverChannel.readPendingMessageNow(SocketBootstrapServerTimeout);

  if (result != IOResult::Success) {
    /*
     *  Must be able to read the response messages successfully from the
     *  bootstrap server.
     */
    return result;
  }

  return registrationResult ? IOResult::Success : IOResult::Failure;
}

std::shared_ptr<Channel> BootstrapServerAcquireAdvertised(
    const std::string& name) {
  if (name.length() == 0) {
    return nullptr;
  }

  auto handle = SocketChannel::CreateClientHandle(SocketBootstrapServerName);

  if (handle == -1) {
    /*
     *  Could not connect to the bootstrap server.
     */
    return nullptr;
  }

  Message::Attachment attachment(handle);

  /*
   *  Paranoid assertion since we have already checked handle validity.
   */
  RL_ASSERT(attachment.isValid());

  Channel serverChannel(std::move(attachment));

  Message requestMessage;

  if (requestMessage.encode(name)) {
    /*
     *  Could not encode request message.
     */
    return nullptr;
  }

  Messages messages;
  messages.push_back(std::move(requestMessage));

  IOResult result = serverChannel.sendMessages(std::move(messages));

  if (result != IOResult::Success) {
    /*
     *  Could not send the request message to the bootstrap server.
     */
    return nullptr;
  }

  std::shared_ptr<Channel> resolvedChannel;
  Channel::MessageCallback channelResolutionCallback = [&resolvedChannel](
      Message responseMessage, Namespace*) {
    if (!responseMessage.readCompleted() ||
        responseMessage.attachments().size() != 1) {
      /*
       *  The response does not contain any useful data. Only one channel is
       *  present. Validate this on our end.
       */
      return;
    }

    auto& attachment = responseMessage.attachments()[0];

    if (!attachment.isValid()) {
      /*
       *  The remote channel attachment was not valid.
       */
      return;
    }

    resolvedChannel = std::make_shared<Channel>(std::move(attachment));
  };

  serverChannel.setMessageCallback(channelResolutionCallback);

  result = serverChannel.readPendingMessageNow(SocketBootstrapServerTimeout);

  if (result != IOResult::Success) {
    /*
     *  Could not read a valid response from the server.
     */
    return nullptr;
  }

  return resolvedChannel;
}

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_SOCKET
