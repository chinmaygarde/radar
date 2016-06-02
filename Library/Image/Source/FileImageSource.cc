// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "FileImageSource.h"

namespace rl {
namespace image {

FileImageSource::FileImageSource(core::File file) : _file(std::move(file)) {}

uint8_t* FileImageSource::sourceData() const {
  RL_WIP;
  return nullptr;
}

size_t FileImageSource::sourceDataSize() const {
  RL_WIP;
  return 0;
}

}  // namespace image
}  // namespace rl
