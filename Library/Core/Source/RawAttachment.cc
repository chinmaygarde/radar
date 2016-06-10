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

RawAttachment::~RawAttachment() = default;

bool RawAttachment::isValid() const {
  return _handle != kInvalidAttachmentHandle;
}

Attachment::Handle RawAttachment::handle() const {
  return _handle;
}

bool RawAttachment::setHandle(Handle handle) {
  if (_handle != kInvalidAttachmentHandle) {
    /*
     *  Raw attachments make no assumptions about ownership of the handle.
     *  We don't want to get into a situation where a valid handle is
     *  overwritten without anyone getting the chance to collect it correctly.
     */
    return false;
  }

  _handle = handle;
  return true;
}

}  // namespace core
}  // namespace rl
