// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Image/ImageResult.h>

namespace rl {
namespace image {

ImageResult::ImageResult(geom::Size size,
                         Components components,
                         core::Allocation allocation)
    : _success(true),
      _size(size),
      _components(components),
      _allocation(std::move(allocation)) {}

ImageResult::ImageResult() : _success(false) {}

ImageResult::ImageResult(ImageResult&&) = default;

ImageResult& ImageResult::operator=(ImageResult&& other) {
  _success = other._success;
  other._success = false;

  _size = other._size;
  other._size = geom::Size{};

  _components = other._components;
  other._components = Components::Invalid;

  _allocation = std::move(other._allocation);

  return *this;
}

bool ImageResult::wasSuccessful() const {
  return _success;
}

const geom::Size& ImageResult::size() const {
  return _size;
}

ImageResult::Components ImageResult::components() const {
  return _components;
}

const core::Allocation& ImageResult::allocation() const {
  return _allocation;
}

ImageResult::~ImageResult() = default;

}  // namespace image
}  // namespace rl
