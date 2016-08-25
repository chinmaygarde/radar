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
#include "MachPort.h"
#endif

namespace rl {
namespace core {

static const Attachment::Handle kInvalidFileHandle = -1;

FileHandle::FileHandle() : _handle(kInvalidFileHandle) {}

FileHandle::FileHandle(RawAttachment attachment) : _handle(kInvalidFileHandle) {
  auto handle = attachment.takeHandle();

#if RL_CHANNELS == RL_CHANNELS_MACH
  /*
   *  Mach channels send descriptors via port rights that need morphing back to
   *  descriptors.
   */
  handle = MachFileDescriptorFromPort(handle, true);
#endif

  _handle = handle;
}

int AccessTypeToOpenType(FileHandle::AccessType type) {
  switch (type) {
    case FileHandle::AccessType::Read:
      return O_RDONLY;
    case FileHandle::AccessType::Write:
      return O_WRONLY;
    case FileHandle::AccessType::ReadWrite:
      return O_RDWR;
  }

  return O_RDONLY;
}

FileHandle::FileHandle(const URI& uri, AccessType type)
    : _handle(kInvalidFileHandle) {
  if (!uri.isValid()) {
    return;
  }

  auto fileName = uri.filesystemRepresentation();

  _handle = RL_TEMP_FAILURE_RETRY(
      ::open(fileName.c_str(), AccessTypeToOpenType(type)));

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

Attachment::MessageHandle FileHandle::messageHandle() const {
#if RL_CHANNELS == RL_CHANNELS_MACH
  /*
   *  Mach channels send descriptors via port rights that need morphing back to
   *  descriptors.
   */
  Attachment::MessageHandle::Collector collector =
      [](Attachment::Handle handle) {  //
        MachPort::Dereference(handle, MachPort::Type::Send);
      };

  return Attachment::MessageHandle{
      MachPortFromFileDescriptor(_handle),  // descriptor
      collector                             // collector
  };

#endif

  return Attachment::MessageHandle{_handle};
}

size_t FileHandle::write(const uint8_t* bytes, size_t length) {
  int written = RL_TEMP_FAILURE_RETRY(::write(_handle, bytes, length));
  return written == -1 ? 0 : written;
}

}  // namespace core
}  // namespace rl
