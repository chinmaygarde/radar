// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_CORE_MACHPORT_H_
#define RADARLOVE_CORE_MACHPORT_H_

#include <Core/Core.h>

namespace rl {
namespace core {

class MachPort {
 public:
  using Handle = uint32_t;

  explicit MachPort(size_t queueLimit);

  explicit MachPort(const Message::Attachment& attachment);

  ~MachPort();

  Handle portHandle() const;
  Handle setHandle() const;

  IOResult sendMessages(Messages&& messages, ClockDurationNano timeout);

  IOReadResult readMessage(ClockDurationNano timeout);

  bool doTerminate();

  static void LogRights(Handle handle);

 private:
  Handle _setHandle;
  Handle _handle;

  void setupWithPortHandle(Handle handle);

  RL_DISALLOW_COPY_AND_ASSIGN(MachPort);
};

}  // namespace core
}  // namespace rl

#endif  // RADARLOVE_CORE_MACHPORT_H_
