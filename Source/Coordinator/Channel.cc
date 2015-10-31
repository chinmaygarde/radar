// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Coordinator/Channel.h>

namespace rl {
namespace coordinator {

Channel::Channel() : core::Channel(), _transactionMessage(new core::Message()) {
}

core::Message& Channel::transactionMessage() {
  return *_transactionMessage;
}

bool Channel::flushTransaction() {
  RL_ASSERT(_transactionMessage);

  core::Messages messages;
  messages.push_back(std::move(*_transactionMessage));

  auto reset = core::make_unique<core::Message>();
  _transactionMessage.swap(reset);

  return sendMessages(std::move(messages));
}

}  // namespace coordinator
}  // namespace rl
