/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Core/Message.h>
#include <unordered_map>

namespace rl {
namespace core {

using ProtocolPayloadIdentifier = uint64_t;

class Protocol {
 public:
  using Response = std::function<void(IOResult, Message)>;

  /**
   *  @param isVendor if this instance of the protocol handle serves as the
   *                  vendor (else client)
   */
  Protocol(bool isVendor);

  std::shared_ptr<EventLoopSource> source();

  void sendRequest(Response response);

  virtual ~Protocol();

 protected:
  using ResponsePayloadHandler = std::function<bool(Message& responseMessage)>;

  virtual std::string advertisementName() const = 0;

  virtual void onRequest(Message requestMessage,
                         std::unique_ptr<Channel> replyChannel,
                         ProtocolPayloadIdentifier identifier) = 0;

  virtual bool populateRequestPayload(Message& message) = 0;

  IOResult fulfillRequest(ProtocolPayloadIdentifier identifier,
                          std::unique_ptr<Channel> replyChannel,
                          ResponsePayloadHandler handler);

 private:
  std::shared_ptr<Channel> _channel;
  std::unordered_map<ProtocolPayloadIdentifier, Response> _pendingResponses;
  bool _isVendor;
  bool _isAdvertising;

  void onChannelMessage(Message message, Namespace* ns);

  void startAdvertisingWithBootstrapServerIfNecessary();

  RL_DISALLOW_COPY_AND_ASSIGN(Protocol);
};

}  // namespace core
}  // namespace rl
