// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/FileHandle.h>

#include <fcntl.h>
#include <unistd.h>

namespace rl {
namespace core {

FileHandle::FileHandle(const URI& uri) : _handle(-1) {
  if (!uri.isValid()) {
    return;
  }

  _handle = RL_TEMP_FAILURE_RETRY(
      ::open(uri.filesystemRepresentation().c_str(), O_RDONLY));
}

FileHandle::FileHandle(FileHandle&& other) : _handle(other._handle) {
  other._handle = -1;
}

FileHandle::~FileHandle() {
  if (_handle != -1) {
    RL_TEMP_FAILURE_RETRY_AND_CHECK(::close(_handle));
    _handle = -1;
  }
}

bool FileHandle::isValid() const {
  return _handle != -1;
}

FileHandle::Handle FileHandle::handle() const {
  return _handle;
}

}  // namespace core
}  // namespace rl
