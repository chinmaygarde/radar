// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Texture.h"

namespace rl {
namespace compositor {

Texture::Texture(image::Image image)
    : _image(image), _state(State::Compressed) {}

Texture::~Texture() = default;

Texture::Texture(Texture&&) = default;

Texture::State Texture::state() const {
  return _state;
}

bool Texture::uncompress() {
  if (_state != State::Compressed) {
    return false;
  }

  auto result = _image.decode();

  if (result.wasSuccessful()) {
    _state = State::Uncompressed;
    _imageResult = std::move(result);
    return true;
  }

  return false;
}

bool Texture::uploadToVRAM() {
  if (_state != State::Uncompressed) {
    return false;
  }

  RL_WIP;

  return false;
}

}  // namespace compositor
}  // namespace rl
