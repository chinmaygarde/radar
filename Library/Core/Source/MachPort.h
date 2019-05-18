/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include "Config.h"

#if RL_CHANNELS == RL_CHANNELS_MACH

#include <Core/IOResult.h>
#include <Core/Macros.h>
#include <Core/RawAttachment.h>
#include <mach/mach.h>

namespace rl {
namespace core {

class MachPort final : public Attachment {
 public:
  enum class Type {
    None,
    SendReceive,
    Send,
    PortSet,
  };

  MachPort(Type type);

  MachPort(RawAttachment attachment);

  ~MachPort();

  bool insertMember(const MachPort& member);

  bool extractMember(const MachPort& member);

  bool setQueueLimit(size_t limit);

  void logRights() const;

  static void LogRights(mach_port_name_t name);

  bool isDeallocated() const;

  Type type() const;

  mach_port_name_t name() const;

  IOResult sendMessages(Messages&& messages,
                        ClockDurationNano requestedTimeout) const;

  IOReadResult receiveMessage(ClockDurationNano requestedTimeout) const;

  bool isValid() const;

  Handle handle() const override;

 private:
  friend class FileHandle;
  friend class MachPayload;
  friend class MachBootstrapClientProvider;

  mach_port_name_t _name = MACH_PORT_NULL;
  Type _type = Type::None;

  static void Dereference(mach_port_name_t name, Type type);

  RL_DISALLOW_COPY_AND_ASSIGN(MachPort);
};

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_MACH
