// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_COMPOSITOR_COMPOSITORCHANNEL_H__
#define __RADARLOVE_COMPOSITOR_COMPOSITORCHANNEL_H__

#include <Core/Core.h>

namespace rl {

class CompositorChannel : public Channel {
 public:
  CompositorChannel();

  Message& transactionMessage();

  bool flushTransaction();

 private:
  std::unique_ptr<Message> _transactionMessage;

  DISALLOW_COPY_AND_ASSIGN(CompositorChannel);
};

}  // namespace rl

#endif  // __RADARLOVE_COMPOSITOR_COMPOSITORCHANNEL_H__