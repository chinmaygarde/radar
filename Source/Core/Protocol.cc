// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Protocol.h>

namespace rl {
namespace core {

static ProtocolPayloadIdentifier _LastProtocolPayloadIdentifier = 0;

class ProtocolPayloadHeader {
 public:
  ProtocolPayloadHeader()
      : _isRequest(true), _identifier(++_LastProtocolPayloadIdentifier){};

  ProtocolPayloadHeader(ProtocolPayloadIdentifier identifier)
      : _isRequest(false), _identifier(identifier) {}

  bool isRequest() const { return _isRequest; }

  bool identifier() const { return _identifier; }

 private:
  bool _isRequest;
  uint64_t _identifier;

  RL_DISALLOW_COPY_AND_ASSIGN(ProtocolPayloadHeader);
};

Protocol::Protocol() : _isVendor(true), _channel(std::make_shared<Channel>()) {
  _channel->setMessageCallback(
      std::bind(&Protocol::onChannelMessage, this, std::placeholders::_1));
}

std::shared_ptr<EventLoopSource> Protocol::source() {
  return _isVendor ? _channel->source() : nullptr;
}

void Protocol::onChannelMessage(Message message) {
  ProtocolPayloadHeader header;

  if (!message.decode(header)) {
    return;
  }

  /*
   *  A Request
   */
  if (header.isRequest()) {
    onRequest(std::move(message), header.identifier());
    return;
  }

  /*
   *  A response
   */
  auto found = _pendingResponses.find(header.identifier());
  RL_ASSERT_MSG(found != _pendingResponses.end(),
                "Must get a response for a valid request");
  found->second(IOResult::Success, std::move(message));
  auto erased = _pendingResponses.erase(header.identifier());
  RL_ASSERT(erased == 1);
}

void Protocol::sendRequest(Response response) {
  RL_ASSERT(response);

  ProtocolPayloadHeader header(true /* request */);

  Message message;

  if (!message.encode(header)) {
    return response(IOResult::Failure, Message{});
  }

  if (!populateRequestPayload(message)) {
    return response(IOResult::Failure, Message{});
  }

  Messages messages;
  messages.emplace_back(std::move(message));

  auto sendResult = _channel->sendMessages(std::move(messages));
  if (sendResult != IOResult::Success) {
    return response(sendResult, Message{});
  }

  auto result = _pendingResponses.insert({header.identifier(), response});
  RL_ASSERT(result.second); /* Paranoid check against identifier collisions */
}

IOResult Protocol::fulfillRequest(ProtocolPayloadIdentifier identifier,
                                  ResponsePayloadHandler handler) {
  ProtocolPayloadHeader header(identifier);
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
  return _channel->sendMessages(std::move(messages));
}

Protocol::~Protocol() {
  for (const auto& pending : _pendingResponses) {
    pending.second(core::IOResult::Timeout, Message{});
  }
}

}  // namespace core
}  // namespace rl
