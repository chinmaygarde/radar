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

  using ReadResult = std::pair<EventLoopSource::IOHandlerResult, Message>;

  explicit MachPort(size_t queueLimit = 1024);
  ~MachPort();

  Handle portHandle() const;
  Handle setHandle() const;

  EventLoopSource::IOHandlerResult sendMessages(Messages&& messages,
                                                ClockDurationNano timeout);

  ReadResult readMessage(ClockDurationNano timeout);

  bool doTerminate();

 private:
  Handle _setHandle;
  Handle _handle;

  RL_DISALLOW_COPY_AND_ASSIGN(MachPort);
};

}  // namespace core
}  // namespace rl

#endif  // RADARLOVE_CORE_MACHPORT_H_
