// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if RL_CHANNELS == RL_CHANNELS_MACH

#include "MachBootstrapClientProvider.h"

#include <Foundation/Foundation.h>
#include <Core/RawAttachment.h>

namespace rl {
namespace core {

MachBootstrapClientProvider::MachBootstrapClientProvider() = default;

MachBootstrapClientProvider::~MachBootstrapClientProvider() = default;

IOResult MachBootstrapClientProvider::doAdvertise(
    const std::string& name,
    std::shared_ptr<Channel> channel) {
  @autoreleasepool {
    auto portName =
        static_cast<uint32_t>(channel->attachment()->attachmentHandle());

    auto port = [NSMachPort portWithMachPort:portName];

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

std::shared_ptr<Channel> MachBootstrapClientProvider::doAcquire(
    const std::string& name) {
  @autoreleasepool {
    auto portName = [NSString stringWithUTF8String:name.c_str()];

    NSMachPort* port = reinterpret_cast<NSMachPort*>(
        [[NSMachBootstrapServer sharedInstance] portForName:portName]);

    if (!port.isValid) {
      return nullptr;
    }

    return std::make_shared<core::Channel>(core::RawAttachment{port.machPort});
  }

  return nullptr;
}

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_MACH
