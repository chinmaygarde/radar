// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Image/ImageResult.h>

namespace rl {
namespace image {

ImageResult::ImageResult(geom::Size size,
                         Components components,
                         std::unique_ptr<core::Allocation> allocation)
    : _success(true),
      _size(size),
      _components(components),
      _allocation(std::move(allocation)) {
  RL_ASSERT(_allocation);
}

ImageResult::ImageResult() : _success(false) {}

ImageResult::ImageResult(ImageResult&&) = default;

bool ImageResult::wasSuccessful() const {
  return _success;
}

const geom::Size& ImageResult::size() const {
  return _size;
}

Components ImageResult::components() const {
  return _components;
}

const std::unique_ptr<core::Allocation>& ImageResult::allocation() const {
  return _allocation;
}

ImageResult::~ImageResult() {}

}  // namespace image
}  // namespace rl
