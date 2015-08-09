// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Interface/InterfaceTransaction.h>

namespace rl {

InterfaceTransaction::InterfaceTransaction() : _patch() {
}

Patch& InterfaceTransaction::patch() {
  return _patch;
}

void InterfaceTransaction::commit(Channel& channel) {
  if (_patch.hasMessage()) {
    Messages messages;
    messages.emplace_back(_patch.flatten());
    channel.sendMessages(std::move(messages));
  }
}

}  // namespace rl
