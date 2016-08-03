// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Attachment.h>

namespace rl {
namespace core {

Attachment::MessageHandle::MessageHandle(Handle handle) : _handle(handle) {}

Attachment::MessageHandle::MessageHandle(Handle handle, Collector collector)
    : _handle(handle), _collector(collector) {}

Attachment::MessageHandle::MessageHandle(MessageHandle&& other)
    : _handle(other._handle), _collector(other._collector) {
  other._handle = -1;
  other._collector = nullptr;
}

Attachment::MessageHandle::~MessageHandle() {
  if (_collector) {
    _collector(_handle);
  }
  _handle = -1;
}

Attachment::Handle Attachment::MessageHandle::handle() const {
  return _handle;
}

Attachment::MessageHandle Attachment::messageHandle() const {
  return MessageHandle{handle()};
}

}  // namespace core
}  // namespace rl
