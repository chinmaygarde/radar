// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Coordinator/InterfaceAcquisitionProtocol.h>

namespace rl {
namespace coordinator {

InterfaceAcquisitionProtocol::InterfaceAcquisitionProtocol(
    InterfaceChannelVendor vendor)
    : _vendor(vendor), core::Protocol(true) {
  RL_ASSERT(_vendor);
}

InterfaceAcquisitionProtocol::InterfaceAcquisitionProtocol()
    : core::Protocol(false) {}

void InterfaceAcquisitionProtocol::onRequest(
    core::Message message,
    std::unique_ptr<core::Channel>
        replyChannel,
    core::ProtocolPayloadIdentifier identifier) {
  std::shared_ptr<core::Channel> channel;

  /*
   *  We don't send any extra information for service acquisition. So we
   *  should not expect anything while servicing the request either.
   */
  if (message.readCompleted()) {
    channel = _vendor();
  }

  auto result = fulfillRequest(
      identifier, std::move(replyChannel), [&channel](core::Message& message) {
        if (channel == nullptr) {
          return false;
        }

        const auto attachment = channel->asMessageAttachment();

        if (!attachment.isValid()) {
          return false;
        }

        return message.addAttachment(channel->asMessageAttachment());
      });

  RL_ASSERT(result == core::IOResult::Success);
}

bool InterfaceAcquisitionProtocol::populateRequestPayload(
    core::Message& message) {
  /*
   *  There is nothing extra to encode
   */
  return true;
}

std::string InterfaceAcquisitionProtocol::advertisementName() const {
  return "com.radar.interface_acquisition";
}

}  // namespace coordinator
}  // namespace rl
