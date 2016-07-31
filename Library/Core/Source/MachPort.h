// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_CORE_MACHPORT_H_
#define RADAR_CORE_MACHPORT_H_

#include "Config.h"

#if RL_CHANNELS == RL_CHANNELS_MACH

#include <Core/Macros.h>
#include <Core/IOResult.h>
#include <Core/Attachment.h>

#include <mach/mach.h>

namespace rl {
namespace core {

class MachPort : public Attachment {
 public:
  enum class Type {
    None,
    SendReceive,
    Send,
    PortSet,
  };

  MachPort(Type type);

  MachPort(Type type, mach_port_name_t name);

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

  bool isValid() const override;

  RL_WARN_UNUSED_RESULT
  Handle takeAttachmentHandle() override;

  Handle attachmentHandle() const override;

 private:
  mach_port_name_t _name;
  Type _type;

  RL_DISALLOW_COPY_AND_ASSIGN(MachPort);
};

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_MACH

#endif  // RADAR_CORE_MACHPORT_H_
