// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "DataImageSource.h"

namespace rl {
namespace image {

DataImageSource::DataImageSource(std::unique_ptr<core::Allocation> allocation)
    : _allocation(std::move(allocation)) {}

uint8_t* DataImageSource::sourceData() const {
  if (!_allocation) {
    return nullptr;
  }

  return _allocation->data();
}

size_t DataImageSource::sourceDataSize() const {
  if (!_allocation) {
    return 0;
  }

  return _allocation->size();
}

}  // namespace image
}  // namespace rl
