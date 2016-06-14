// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/FileHandle.h>
#include <Core/Utilities.h>

#include <fcntl.h>
#include <unistd.h>

#if RL_CHANNELS == RL_CHANNELS_MACH
#include "MachFilePort.h"
#endif

namespace rl {
namespace core {

static const FileHandle::Handle kInvalidFileHandle = -1;

FileHandle::FileHandle(RawAttachment attachment)
    :
#if RL_CHANNELS == RL_CHANNELS_MACH
      _handle(MachFilePort::DescriptorFromPort(attachment.takeHandle(), true))
#else
      _handle(attachment.takeHandle())
#endif
{
}

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
#if RL_CHANNELS == RL_CHANNELS_MACH
  /*
   *  Does this need extra locking to initialize?
   */
  if (_port == nullptr) {
    _port = make_unique<MachFilePort>(_handle);
  }
  return _port->name();
#else
  return _handle;
#endif
}

Attachment::Handle FileHandle::takeHandle() {
  auto handle = _handle;
  _handle = kInvalidFileHandle;
#if RL_CHANNELS == RL_CHANNELS_MACH
  _port = nullptr;
#endif
  return handle;
}

}  // namespace core
}  // namespace rl
