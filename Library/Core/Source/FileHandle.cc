// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/FileHandle.h>
#include <Core/Utilities.h>

#include <fcntl.h>
#include <unistd.h>

#include "Config.h"

#if RL_CHANNELS == RL_CHANNELS_MACH
#include "MachFilePort.h"
#endif

namespace rl {
namespace core {

static const Attachment::Handle kInvalidFileHandle = -1;

FileHandle::FileHandle() : _handle(kInvalidFileHandle) {}

FileHandle::FileHandle(RawAttachment attachment) : _handle(kInvalidFileHandle) {
  auto handle = attachment.takeHandle();

#if RL_CHANNELS == RL_CHANNELS_MACH
  /*
   *  Mach channels send descriptors via port rights that need morphing to
   *  descriptors.
   */
  handle = MachFilePort::DescriptorFromPort(handle, true);
#endif

  _handle = handle;
}

FileHandle::FileHandle(const URI& uri) : _handle(kInvalidFileHandle) {
  if (!uri.isValid()) {
    return;
  }

  auto fileName = uri.filesystemRepresentation();

  _handle = RL_TEMP_FAILURE_RETRY(::open(fileName.c_str(), O_RDONLY));

  if (_handle == kInvalidFileHandle) {
    RL_LOG("Could not open file: '%s'", fileName.c_str());
    RL_LOG_ERRNO();
  }
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

}  // namespace core
}  // namespace rl
