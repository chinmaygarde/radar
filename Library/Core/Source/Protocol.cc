// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Protocol.h>

namespace rl {
namespace core {

static ProtocolPayloadIdentifier _LastProtocolPayloadIdentifier = 0;

class ProtocolPayloadHeader {
 public:
  enum class Type : uint8_t {
    Request,
    Response,
  };

  ProtocolPayloadHeader(Type type,
                        ProtocolPayloadIdentifier identifier,
                        bool autoAssignID)
      : _type(type),
        _identifier(autoAssignID ? ++_LastProtocolPayloadIdentifier
                                 : identifier) {}

  Type type() const { return _type; }

  uint64_t identifier() const { return _identifier; }

 private:
  Type _type;
  uint64_t _identifier;

  RL_DISALLOW_COPY_AND_ASSIGN(ProtocolPayloadHeader);
};

static_assert(rl_trivially_copyable(ProtocolPayloadHeader),
              "The protocol payload header must be trivially copyable");

Protocol::Protocol(bool isVendor)
    : _channel(std::make_shared<Channel>()),
      _isVendor(isVendor),
      _isAdvertising(false) {
  _channel->setMessageCallback(std::bind(&Protocol::onChannelMessage, this,
                                         std::placeholders::_1,
                                         std::placeholders::_2));
}

std::shared_ptr<EventLoopSource> Protocol::source() {
  startAdvertisingWithBootstrapServerIfNecessary();
  return _channel->source();
}

void Protocol::onChannelMessage(Message message, Namespace* ns) {
  ProtocolPayloadHeader header(ProtocolPayloadHeader::Type::Request, 0, false);

  if (!message.decode(header, ns)) {
    return;
  }

  /*
   *  A Request
   */
  if (header.type() == ProtocolPayloadHeader::Type::Request) {
    RL_ASSERT_MSG(_isVendor, "Only protocol vendors may service requests");

    /*
     *  The first attachment in the request messages is the reply channel
     */
    if (message.attachments().size() != 1) {
      return;
    }

    auto replyChannel =
        make_unique<Channel>(Message::Attachment{message.attachments()[0]});

    onRequest(std::move(message), std::move(replyChannel), header.identifier());

    return;
  }

  /*
   *  A response
   */
  RL_ASSERT_MSG(!_isVendor, "Protocol vendors cannot service responses");
  auto found = _pendingResponses.find(header.identifier());
  RL_ASSERT_MSG(found != _pendingResponses.end(),
                "Must get a response for a valid request");
  found->second(IOResult::Success, std::move(message));
  auto erased = _pendingResponses.erase(header.identifier());
  RL_ASSERT(erased == 1);
}

void Protocol::sendRequest(Response response) {
  RL_ASSERT(response);

  /*
   *  Try to acquire the advertised channel for this protocol from the boostrap
   *  server
   */
  auto remoteChannel = BootstrapServerAcquireAdvertised(advertisementName());

  if (remoteChannel == nullptr) {
    /*
     *  No one is advertising itself as a protocol vendor. Try later.
     */
    return response(IOResult::Timeout, Message{});
  }

  /*
   *  Construct the protocol message header
   */
  ProtocolPayloadHeader header(ProtocolPayloadHeader::Type::Request, 0, true);

  Message message;

  if (!message.encode(header)) {
    return response(IOResult::Failure, Message{});
  }

  if (!populateRequestPayload(message)) {
    return response(IOResult::Failure, Message{});
  }

  if (!message.addAttachment(_channel->asMessageAttachment())) {
    return response(IOResult::Failure, Message{});
  }

  Messages messages;
  messages.emplace_back(std::move(message));

  auto sendResult = remoteChannel->sendMessages(std::move(messages));
  if (sendResult != IOResult::Success) {
    return response(sendResult, Message{});
  }

  auto result = _pendingResponses.insert({header.identifier(), response});
  RL_ASSERT(result.second); /* Paranoid check against identifier collisions */
}

IOResult Protocol::fulfillRequest(ProtocolPayloadIdentifier identifier,
                                  std::unique_ptr<Channel> replyChannel,
                                  ResponsePayloadHandler handler) {
  ProtocolPayloadHeader header(ProtocolPayloadHeader::Type::Response,
                               identifier, false);
  Message message;

  if (!message.encode(header)) {
    return IOResult::Failure;
  }

  if (!handler) {
    return IOResult::Failure;
  }

  if (!handler(message)) {
    return IOResult::Failure;
  }

  Messages messages;
  messages.emplace_back(std::move(message));
  return replyChannel->sendMessages(std::move(messages));
}

void Protocol::startAdvertisingWithBootstrapServerIfNecessary() {
  if (!_isVendor) {
    return;
  }

  if (!_isAdvertising) {
    _isAdvertising = BootstrapServerAdvertise(advertisementName(), _channel) ==
                     IOResult::Success;
  }
}

Protocol::~Protocol() {
  _channel->setMessageCallback(nullptr);

  for (const auto& pending : _pendingResponses) {
    pending.second(core::IOResult::Timeout, Message{});
  }
}

}  // namespace core
}  // namespace rl
