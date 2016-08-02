// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/RawAttachment.h>

namespace rl {
namespace core {

static const Attachment::Handle kInvalidAttachmentHandle = -1;

RawAttachment::RawAttachment() : _handle(kInvalidAttachmentHandle) {}

RawAttachment::RawAttachment(Handle handle, Collector collector)
    : _handle(handle), _collector(collector) {}

RawAttachment::RawAttachment(RawAttachment&& other)
    : _handle(other._handle), _collector(other._collector) {
  other._handle = kInvalidAttachmentHandle;
  other._collector = nullptr;
}

RawAttachment& RawAttachment::operator=(RawAttachment&& other) {
  std::swap(_handle, other._handle);
  std::swap(_collector, other._collector);
  return *this;
}

RawAttachment::~RawAttachment() {
  if (_collector) {
    _collector(_handle);
  }
}

Attachment::Handle RawAttachment::handle() const {
  return _handle;
}

Attachment::Handle RawAttachment::takeHandle() {
  auto handle = _handle;
  _handle = kInvalidAttachmentHandle;
  _collector = nullptr;
  return handle;
}

}  // namespace core
}  // namespace rl
