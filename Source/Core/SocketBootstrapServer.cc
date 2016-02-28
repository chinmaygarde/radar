// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if RL_CHANNELS == RL_CHANNELS_SOCKET

#include <Core/BootstrapServer.h>
#include <Core/SocketBootstrapServer.h>
#include <Core/SocketChannel.h>
#include <Core/Latch.h>

#include <thread>

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

bool SocketBootstrapServer::attemptRegistration(
    const std::string& name,
    Message::Attachment channelAttachment) {
  if (name.length() == 0 || !channelAttachment.isValid()) {
    /*
     *  The new registration has invalid arguments.
     */
    return false;
  }

  auto emplaced = _channels.emplace(
      name, std::make_shared<Channel>(std::move(channelAttachment)));

  return emplaced.second;
}

std::shared_ptr<Channel> SocketBootstrapServer::acquireRegistration(
    const std::string& name) {
  if (name.length() == 0) {
    /*
     *  Invalid name argument.
     */
    return nullptr;
  }

  auto found = _channels.find(name);

  if (found == _channels.end()) {
    return nullptr;
  }

  return found->second;
}

IOResult SocketBootstrapServer::sendBoostrapResponse(
    Channel& replyChannel,
    BootstrapResponse response) {
  Message responseMessage;

  if (!responseMessage.encode(response.first)) {
    return IOResult::Failure;
  }

  if (response.second) {
    auto attachment = response.second->asMessageAttachment();

    if (!attachment.isValid()) {
      return IOResult::Failure;
    }

    responseMessage.addAttachment(std::move(attachment));
  }

  Messages messages;
  messages.emplace_back(std::move(responseMessage));

  return replyChannel.sendMessages(std::move(messages),
                                   SocketBootstrapServerTimeout);
}

SocketBootstrapServer::BootstrapResponse
SocketBootstrapServer::processBootstrapMessageRequest(
    Message&& requestMessage) {
  /*
   *  A valid bootstrap client message always contains at least one string
   *  name.
   */
  std::string name;
  if (!requestMessage.decode(name)) {
    return BootstrapResponse(false, nullptr);
  }

  switch (requestMessage.attachments().size()) {
    case 0:
      /*
       *  If the request message contains no attachements, it is a channel
       *  acquisition attempt.
       */
      if (auto channel = acquireRegistration(name)) {
        return BootstrapResponse(true, channel);
      }
      break;
    case 1:
      /*
       *  If the request message contains one attachement, it is an
       *  advertisement attempt and the attachment is the channel being
       *  advertised.
       */
      if (attemptRegistration(name, requestMessage.attachments()[0])) {
        /*
         *  Channel registration was successful.
         */
        return BootstrapResponse(true, nullptr);
      }
      break;
    default:
      /*
       *  Everything else is a failure.
       */
      break;
  }

  return BootstrapResponse(false, nullptr);
}

IOResult SocketBootstrapServer::processBootstrapMessageRequestAndReply(
    Channel& replyChannel,
    Message&& requestMessage) {
  auto result = processBootstrapMessageRequest(std::move(requestMessage));
  return sendBoostrapResponse(replyChannel, result);
}

IOResult SocketBootstrapServer::onListenReadResult(
    EventLoopSource::Handle readHandle) {
  auto client = SocketChannel::AcceptClientHandle(
      static_cast<SocketChannel::Handle>(readHandle));

  if (!client) {
    return IOResult::Failure;
  }

  Channel::MessageCallback messageCallback = [&](Message requestMessage,
                                                 Namespace* ns) {
    processBootstrapMessageRequestAndReply(*client, std::move(requestMessage));
  };

  client->setMessageCallback(messageCallback);
  client->readPendingMessageNow(SocketBootstrapServerTimeout);

  /*
   *  No matter what happens on the client channel. We want the connection to be
   *  immediately terminated. Signal failure here.
   */
  return IOResult::Failure;
}

IOResult BootstrapServerAdvertise(const std::string& name,
                                  std::shared_ptr<Channel> advertisedChannel) {
  /*
   *  ==========================================================================
   *  Step 1: Sanitize arguments and acquire connection to the bootstrap server.
   *  ==========================================================================
   */
  if (name.length() == 0 || !advertisedChannel) {
    return IOResult::Failure;
  }

  Message::Attachment advertisedChannelAttachment =
      advertisedChannel->asMessageAttachment();

  if (!advertisedChannelAttachment.isValid()) {
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

  /*
   *  ==========================================================================
   *  Step 2: Send the advertisement message.
   *  ==========================================================================
   */

  Message requestMessage;

  if (!requestMessage.encode(name) ||
      !requestMessage.addAttachment(std::move(advertisedChannelAttachment))) {
    /*
     *  Could not encode the request message.
     */
    return IOResult::Failure;
  }

  Messages messages;
  messages.push_back(std::move(requestMessage));

  auto result = serverChannel.sendMessages(std::move(messages),
                                           SocketBootstrapServerTimeout);

  if (result != IOResult::Success) {
    /*
     *  Sending the request message must be successful.
     */
    return result;
  }

  /*
   *  ==========================================================================
   *  Step 3: Read the response back from the bootstrap server.
   *  ==========================================================================
   */

  bool registrationResult = false;

  Channel::MessageCallback responseCallback = [&registrationResult](
      Message responseMessage, Namespace* ns) {
    if (!responseMessage.decode(registrationResult, ns)) {
      registrationResult = false;
      return;
    }
  };

  serverChannel.setMessageCallback(responseCallback);

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
  /*
   *  ==========================================================================
   *  Step 1: Sanitize arguments and acquire connection to the bootstrap server.
   *  ==========================================================================
   */
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

  /*
   *  ==========================================================================
   *  Step 2: Send acquisition request message.
   *  ==========================================================================
   */

  Message requestMessage;

  if (!requestMessage.encode(name)) {
    /*
     *  Could not encode request message.
     */
    return nullptr;
  }

  Messages messages;
  messages.push_back(std::move(requestMessage));

  IOResult result = serverChannel.sendMessages(std::move(messages),
                                               SocketBootstrapServerTimeout);

  if (result != IOResult::Success) {
    /*
     *  Could not send the request message to the bootstrap server.
     */
    return nullptr;
  }

  /*
   *  ==========================================================================
   *  Step 3: Read the response from the bootstrap server.
   *  ==========================================================================
   */

  std::shared_ptr<Channel> resolvedChannel;
  Channel::MessageCallback channelResolutionCallback = [&resolvedChannel](
      Message responseMessage, Namespace* ns) {
    bool result = false;

    if (!responseMessage.decode(result, ns)) {
      return;
    }

    if (!result || responseMessage.attachments().size() != 1) {
      /*
       *  The response was failure. Or in case or success, the one channel is
       *  not present.
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

static std::mutex LocalInstanceMutex;
using BootstrapServerInstance =
    std::pair<std::unique_ptr<SocketBootstrapServer> /* server */,
              std::shared_ptr<EventLoopSource> /* termination source */>;

BootstrapServerInstance LocalInstance;

bool BootstrapServerSetup() {
  std::lock_guard<std::mutex> lock(LocalInstanceMutex);

  if (LocalInstance.first) {
    return true;
  }

  Latch readyLatch(1);

  LocalInstance.first = make_unique<SocketBootstrapServer>();
  LocalInstance.second = EventLoopSource::Trivial();

  std::thread bootstrapServerThread([&]() {
    thread::SetName("rl.bootstrap_server");

    auto loop = EventLoop::Current();

    /*
     *  Add the bootstrap server source.
     */
    loop->addSource(LocalInstance.first->source());

    /*
     *  Add the source used to signal loop termination.
     */
    loop->addSource(LocalInstance.second);

    loop->loop([&]() { readyLatch.countDown(); });
  });

  /*
   *  Wait for the event loop servicing the bootstrap server to be fully ready.
   */
  readyLatch.wait();

  bootstrapServerThread.detach();

  return true;
}

bool BootstrapServerTeardown() {
  std::lock_guard<std::mutex> lock(LocalInstanceMutex);

  if (!LocalInstance.first) {
    return true;
  }

  LocalInstance.second->setWakeFunction([](IOResult) {
    /*
     *  The thread is detached. Terminate its event loop.
     */
    EventLoop::Current()->terminate();
  });

  LocalInstance.first = nullptr;
  LocalInstance.second = nullptr;

  return true;
}

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_SOCKET
