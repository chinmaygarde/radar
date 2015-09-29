// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_CORE_MACHPORT_H_
#define RADARLOVE_CORE_MACHPORT_H_

#include <Core/Core.h>

namespace rl {

class MachPort {
 public:
  using Handle = uint32_t;

  enum Result {
    Failure,
    Success,
    Timeout,
  };

  using ReadResult = std::pair<Result, Messages>;

  explicit MachPort();
  ~MachPort();

  Handle portHandle() const;
  Handle setHandle() const;

  Result sendMessages(Messages&& messages,
                      ClockDurationMilli timeout = ClockDurationMilli::max());
  ReadResult readMessages(
      ClockDurationMilli timeout = ClockDurationMilli::max());

  bool doTerminate();

 private:
  Handle _setHandle;
  Handle _handle;

  RL_DISALLOW_COPY_AND_ASSIGN(MachPort);
};

}  // namespace rl

#endif  // RADARLOVE_CORE_MACHPORT_H_
