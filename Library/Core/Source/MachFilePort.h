// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_CORE_MACHFILEPORT_H_
#define RADAR_CORE_MACHFILEPORT_H_

#include <Core/Macros.h>

namespace rl {
namespace core {

class MachFilePort {
 public:
  RL_WARN_UNUSED_RESULT
  static int DescriptorFromPort(intptr_t name, bool consumePort);

  MachFilePort(int descriptor);

  intptr_t name() const;

  ~MachFilePort();

 private:
  intptr_t _name;

  RL_DISALLOW_COPY_AND_ASSIGN(MachFilePort);
};

}  // namespace core
}  // namespace rl

#endif  // RADAR_CORE_MACHFILEPORT_H_
