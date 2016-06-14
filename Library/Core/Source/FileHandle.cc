// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/FileHandle.h>

#include <fcntl.h>
#include <unistd.h>

namespace rl {
namespace core {

static const FileHandle::Handle kInvalidFileHandle = -1;

FileHandle::FileHandle(RawAttachment attachment)
    : _handle(attachment.takeHandle()) {}

FileHandle::FileHandle(const URI& uri) : _handle(kInvalidFileHandle) {
  if (!uri.isValid()) {
    return;
  }

  _handle = RL_TEMP_FAILURE_RETRY(
      ::open(uri.filesystemRepresentation().c_str(), O_RDONLY));
}

FileHandle::FileHandle(FileHandle&& other) : _handle(other._handle) {
  other._handle = kInvalidFileHandle;
}

FileHandle::~FileHandle() {
  if (_handle != kInvalidFileHandle) {
    RL_TEMP_FAILURE_RETRY_AND_CHECK(::close(_handle));
    _handle = kInvalidFileHandle;
  }
}

bool FileHandle::isValid() const {
  return _handle != kInvalidFileHandle;
}

Attachment::Handle FileHandle::handle() const {
  return _handle;
}

Attachment::Handle FileHandle::takeHandle() {
  auto handle = _handle;
  _handle = kInvalidFileHandle;
  return handle;
}

}  // namespace core
}  // namespace rl
