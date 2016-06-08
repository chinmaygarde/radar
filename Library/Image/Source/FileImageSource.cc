// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "FileImageSource.h"

namespace rl {
namespace image {

FileImageSource::FileImageSource() {}

FileImageSource::FileImageSource(const core::File& file)
    : _mapping(file.map()) {}

uint8_t* FileImageSource::sourceData() const {
  return _mapping.mapping();
}

size_t FileImageSource::sourceDataSize() const {
  return _mapping.size();
}

bool FileImageSource::serialize(core::Message& message) const {
  RL_WIP;
  return false;
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
