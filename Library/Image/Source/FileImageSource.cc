// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "FileImageSource.h"

namespace rl {
namespace image {

FileImageSource::FileImageSource(const core::File& file)
    : _mapping(file.map()) {}

uint8_t* FileImageSource::sourceData() const {
  return _mapping.mapping();
}

size_t FileImageSource::sourceDataSize() const {
  return _mapping.size();
}

}  // namespace image
}  // namespace rl
