// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/FileIOAdapter.h>
#include <unistd.h>

namespace rl {
namespace core {

FileIOAdapter::FileIOAdapter(FileHandle handle) : _handle(std::move(handle)) {}

FileIOAdapter::~FileIOAdapter() = default;

static inline int SeekWhence(FileIOAdapter::Whence whence) {
  switch (whence) {
    case FileIOAdapter::Whence::Set:
      return SEEK_SET;
    case FileIOAdapter::Whence::Current:
      return SEEK_CUR;
    case FileIOAdapter::Whence::End:
      return SEEK_END;
  }

  return SEEK_SET;
}

bool FileIOAdapter::isValid() const {
  return _handle.isValid();
}

bool FileIOAdapter::seek(Whence whence, Offset offset) {
  return ::lseek(_handle.handle(), offset, SeekWhence(whence)) != -1;
}

size_t FileIOAdapter::write(const uint8_t* buffer, size_t size) {
  auto written = RL_TEMP_FAILURE_RETRY(::write(_handle.handle(), buffer, size));
  return written == -1 ? 0 : written;
}

size_t FileIOAdapter::write(const Allocation& allocation) {
  if (!allocation.isReady()) {
    return 0;
  }

  return write(allocation.data(), allocation.size());
}

}  // namespace core
}  // namespace rl
