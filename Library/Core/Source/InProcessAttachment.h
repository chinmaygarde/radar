// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_CORE_INPROCESSATTACHMENT_H_
#define RADARLOVE_CORE_INPROCESSATTACHMENT_H_

#include <Core/Config.h>

#if RL_CHANNELS == RL_CHANNELS_INPROCESS

#include <Core/Macros.h>

namespace rl {
namespace core {

class InProcessAttachment {
 public:
  InProcessAttachment();

  virtual ~InProcessAttachment();

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(InProcessAttachment);
};

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_INPROCESS

#endif  // RADARLOVE_CORE_INPROCESSATTACHMENT_H_
