// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _RADARLOVE_CORE_INPROCESSWAITSET_H__
#define _RADARLOVE_CORE_INPROCESSWAITSET_H__

#include <Core/Macros.h>
#include <Core/WaitSetProvider.h>

namespace rl {

class InProcessWaitSet : public WaitSetProvider {
 public:
  InProcessWaitSet();
  ~InProcessWaitSet();

  EventLoopSource& wait() override;
  WaitSet::Handle handle() const override;

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(InProcessWaitSet);
};

}  // namespace rl

#endif  // _RADARLOVE_CORE_INPROCESSWAITSET_H__
