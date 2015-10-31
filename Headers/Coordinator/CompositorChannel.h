// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_COMPOSITOR_COMPOSITORCHANNEL_H_
#define RADARLOVE_COMPOSITOR_COMPOSITORCHANNEL_H_

#include <Core/Core.h>

namespace rl {
namespace coordinator {

class CompositorChannel : public core::Channel {
 public:
  CompositorChannel();

  core::Message& transactionMessage();

  bool flushTransaction();

 private:
  std::unique_ptr<core::Message> _transactionMessage;

  RL_DISALLOW_COPY_AND_ASSIGN(CompositorChannel);
};

}  // namespace coordinator
}  // namespace rl

#endif  // RADARLOVE_COMPOSITOR_COMPOSITORCHANNEL_H_
