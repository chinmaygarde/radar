// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_COORDINATOR_INTERFACEACQUISITIONPROTOCOL_H_
#define RADARLOVE_COORDINATOR_INTERFACEACQUISITIONPROTOCOL_H_

#include <Core/Core.h>

namespace rl {
namespace coordinator {

class InterfaceAcquisitionProtocol : public core::Protocol {
 public:
  using InterfaceChannelVendor =
      std::function<std::shared_ptr<core::Channel>(void)>;

  InterfaceAcquisitionProtocol(InterfaceChannelVendor vendor);

 private:
  InterfaceChannelVendor _vendor;

  void onRequest(core::Message message,
                 core::ProtocolPayloadIdentifier identifier) override;

  bool populateRequestPayload(core::Message& message) override;

  RL_DISALLOW_COPY_AND_ASSIGN(InterfaceAcquisitionProtocol);
};

}  // namespace coordinator
}  // namespace rl

#endif  // RADARLOVE_COORDINATOR_INTERFACEACQUISITIONPROTOCOL_H_
