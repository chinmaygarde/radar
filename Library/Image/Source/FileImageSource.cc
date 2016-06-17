// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "FileImageSource.h"

#include <Core/Message.h>

namespace rl {
namespace image {

FileImageSource::FileImageSource() {}

FileImageSource::FileImageSource(core::FileHandle fileHandle)
    : _handle(std::make_shared<core::FileHandle>(std::move(fileHandle))),
      _mapping(*_handle) {}

uint8_t* FileImageSource::sourceData() const {
  return _mapping.mapping();
}

size_t FileImageSource::sourceDataSize() const {
  return _mapping.size();
}

bool FileImageSource::serialize(core::Message& message) const {
  if (_handle == nullptr || !_handle->isValid()) {
    return false;
  }

  return message.encode(_handle);
}

bool FileImageSource::deserialize(core::Message& message, core::Namespace* ns) {
  RL_WIP;
  return false;
}

ImageSource::Type FileImageSource::type() const {
  return ImageSource::Type::File;
}

}  // namespace image
}  // namespace rl
