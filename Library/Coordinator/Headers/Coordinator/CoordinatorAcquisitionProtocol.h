// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_COORDINATOR_COORDINATORACQUISITIONPROTOCOL_H_
#define RADARLOVE_COORDINATOR_COORDINATORACQUISITIONPROTOCOL_H_

#include <Core/Core.h>

namespace rl {
namespace coordinator {

/**
 *  The protocol used by the coordinator to vend communication channels to
 *  interested interfaces.
 */
class CoordinatorAcquisitionProtocol : public core::Protocol {
 public:
  using VendorResult = std::pair<std::shared_ptr<core::Channel> /* channel */,
                                 std::string /* tag */>;
  using ChannelVendor = std::function<VendorResult(void)>;

  /**
   *  Create the client side of the coordinator acquisition protocol.
   */
  CoordinatorAcquisitionProtocol();

  /**
   *  Create the vendor side of the coordinator acquisition protocol. The
   *  protocol automatically advertised with the bootstrap server.
   *
   *  @param vendor the channel acquisition callback for this vendor. A channel
   *                will be requested when the bootstrap server notices a
   *                pending advertisement resolution for this protocol.
   */
  CoordinatorAcquisitionProtocol(ChannelVendor vendor);

 private:
  ChannelVendor _vendor;

  std::string advertisementName() const override;

  void onRequest(core::Message requestMessage,
                 std::unique_ptr<core::Channel> replyChannel,
                 core::ProtocolPayloadIdentifier identifier) override;

  bool populateRequestPayload(core::Message& message) override;

  RL_DISALLOW_COPY_AND_ASSIGN(CoordinatorAcquisitionProtocol);
};

}  // namespace coordinator
}  // namespace rl

#endif  // RADARLOVE_COORDINATOR_COORDINATORACQUISITIONPROTOCOL_H_
