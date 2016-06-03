// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if RL_CHANNELS == RL_CHANNELS_MACH

#include <Core/BootstrapServer.h>
#include <Foundation/Foundation.h>

namespace rl {
namespace core {

IOResult BootstrapServerAdvertise(const std::string& name,
                                  std::shared_ptr<core::Channel> channel) {
  if (name.size() == 0) {
    return IOResult::Failure;
  }

  if (!channel) {
    return IOResult::Failure;
  }

  const auto& attachment = channel->attachment();

  if (!attachment.isValid()) {
    return IOResult::Failure;
  }

  @autoreleasepool {
    auto port = [NSMachPort
        portWithMachPort:static_cast<uint32_t>(attachment.handle())];

    if (!port.isValid) {
      return IOResult::Failure;
    }

    NSMachBootstrapServer* server = [NSMachBootstrapServer sharedInstance];
    auto nameString = [NSString stringWithUTF8String:name.c_str()];
    bool advertised = [server registerPort:port name:nameString] == YES;

    return advertised ? IOResult::Success : IOResult::Failure;
  }

  return IOResult::Failure;
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

    return std::make_shared<core::Channel>(core::Attachment{port.machPort});
  }

  return nullptr;
}

bool BootstrapServerSetup() {
  return true;
}

bool BootstrapServerTeardown() {
  return true;
}

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_MACH
