// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/RawAttachment.h>

namespace rl {
namespace core {

static const Attachment::Handle kInvalidAttachmentHandle = -1;

RawAttachment::RawAttachment() : _handle(kInvalidAttachmentHandle) {}

RawAttachment::RawAttachment(Handle handle) : _handle(handle) {}

RawAttachment::RawAttachment(RawAttachment&& other) : _handle(other._handle) {
  other._handle = kInvalidAttachmentHandle;
}

RawAttachment::~RawAttachment() {
  RL_ASSERT_MSG(_handle == kInvalidAttachmentHandle,
                "The raw attachment must be collected correctly.");
}

bool RawAttachment::isValid() const {
  return _handle != kInvalidAttachmentHandle;
}

Attachment::Handle RawAttachment::takeHandle() {
  auto handle = _handle;
  _handle = kInvalidAttachmentHandle;
  return handle;
}

Attachment::Handle RawAttachment::handle() const {
  return _handle;
}

void RawAttachment::setHandle(Handle handle) {
  RL_ASSERT_MSG(_handle == kInvalidAttachmentHandle,
                "The caller must ensure that the handle of this instance is "
                "invalid to avoid incorrect handle lifecycle management");

  _handle = handle;
}

}  // namespace core
}  // namespace rl
