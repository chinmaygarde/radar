// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if RL_CHANNELS == RL_CHANNELS_MACH

#include <Bootstrap/BootstrapServer.h>
#include <Foundation/Foundation.h>

namespace rl {
namespace bootstrap {

bool BootstrapServerAdvertise(const std::string& name,
                              std::shared_ptr<core::Channel>
                                  channel) {
  if (name.size() == 0) {
    return false;
  }

  if (!channel) {
    return false;
  }

  const auto attachment = channel->asMessageAttachment();

  if (!attachment.isValid()) {
    return false;
  }

  @autoreleasepool {
    auto port = [NSMachPort
        portWithMachPort:static_cast<uint32_t>(attachment.handle())];

    if (port == nullptr) {
      return false;
    }

    return
        [[NSMachBootstrapServer sharedInstance]
            registerPort:port
                    name:[NSString stringWithUTF8String:name.c_str()]] == YES;
  }

  return false;
}

std::shared_ptr<core::Channel> BootstrapServerAcquireAdvertised(
    const std::string& name) {
  @autoreleasepool {
    NSMachPort* port =
        reinterpret_cast<NSMachPort*>([[NSMachBootstrapServer sharedInstance]
            portForName:[NSString stringWithUTF8String:name.c_str()]]);

    if (!port.isValid) {
      return nullptr;
    }

    return std::make_shared<core::Channel>(
        core::Message::Attachment{port.machPort});
  }

  return nullptr;
}

bool BoostrapServerStopAdvertising(std::shared_ptr<core::Channel> channel) {
  /*
   *  There is no mach call to stop advertising. We just wait for channel
   *  invalidation.
   */
  return true;
}

}  // namespace bootstrap
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_MACH
