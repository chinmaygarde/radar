// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_CORE_MACHPORT_H_
#define RADAR_CORE_MACHPORT_H_

#include <Core/Macros.h>
#include <Core/IOResult.h>

#include <mach/mach.h>

namespace rl {
namespace core {

class MachPort {
 public:
  enum class Type {
    SendReceive,
    PortSet,
  };

  MachPort(Type type);

  MachPort(Type type, mach_port_name_t name);

  ~MachPort();

  bool insertMember(const MachPort& member);

  bool extractMember(const MachPort& member);

  bool setQueueLimit(size_t limit);

  bool isValid() const;

  void logRights() const;

  mach_port_name_t name() const;

  IOResult sendMessages(Messages&& messages,
                        ClockDurationNano requestedTimeout) const;

  IOReadResult receiveMessage(ClockDurationNano requestedTimeout) const;

 private:
  mach_port_name_t _name;
  Type _type;

  RL_DISALLOW_COPY_AND_ASSIGN(MachPort);
};

}  // namespace core
}  // namespace rl

#endif  // RADAR_CORE_MACHPORT_H_
