/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "Config.h"

#if RL_CHANNELS == RL_CHANNELS_MACH

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

#include <Core/RawAttachment.h>
#include <Foundation/Foundation.h>
#include "MachBootstrapClientProvider.h"
#include "MachPort.h"

namespace rl {
namespace core {

MachBootstrapClientProvider::MachBootstrapClientProvider() = default;

MachBootstrapClientProvider::~MachBootstrapClientProvider() = default;

IOResult MachBootstrapClientProvider::doAdvertise(
    const std::string& name,
    std::shared_ptr<Channel> channel) {
  @autoreleasepool {
    auto portName = static_cast<uint32_t>(channel->attachment()->handle());

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

    RawAttachment::Collector collector = [](Attachment::Handle handle) {
      MachPort::Dereference(handle, MachPort::Type::Send);
    };

    return std::make_shared<Channel>(RawAttachment{port.machPort, collector});
  }

  return nullptr;
}

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_MACH

#pragma clang diagnostic pop
