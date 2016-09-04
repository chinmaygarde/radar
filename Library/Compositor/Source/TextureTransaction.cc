// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "TextureTransaction.h"

namespace rl {
namespace compositor {

TextureTransaction::TextureTransaction() {}

TextureTransaction::~TextureTransaction() = default;

std::shared_ptr<Texture> TextureTransaction::registerTexture(
    std::shared_ptr<Texture> texture) {
  auto cachedTexture = _textureCache.registerTexture(texture);

  RL_ASSERT(cachedTexture != nullptr);

  switch (cachedTexture->state()) {
    case Texture::State::Compressed:
    case Texture::State::Uncompressed:
    default:
      _textures.emplace_back(texture);
      break;
  }

  return cachedTexture;
}

bool TextureTransaction::uncompressImages(core::WorkQueue* workqueue) {
  if (_latch != nullptr) {
    return false;
  }

  _latch = core::make_unique<rl::core::Latch>(_textures.size());

  for (auto& texture : _textures) {
    if (workqueue) {
      auto dispatched = workqueue->dispatch([&]() {
        texture->uncompress();
        _latch->countDown();
      });
      RL_UNUSED(dispatched);
    } else {
      texture->uncompress();
      _latch->countDown();
    }
  }

  return true;
}

bool TextureTransaction::uploadImagesToVRAM() {
  if (_latch == nullptr) {
    return false;
  }

  /*
   *  Make sure all the workqueue work items are done processing.
   */
  _latch->wait();

  /*
   *  Upload to VRAM on the callers thread.
   */
  for (auto& texture : _textures) {
    texture->uploadToVRAM();
  }

  return true;
}

bool TextureTransaction::commit(core::WorkQueue* workqueue) {
  RL_RETURN_IF_FALSE(uncompressImages(workqueue));
  RL_RETURN_IF_FALSE(uploadImagesToVRAM());
  return true;
}

}  // namespace compositor
}  // namespace rl
