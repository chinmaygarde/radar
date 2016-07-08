// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "TextureTransaction.h"

namespace rl {
namespace compositor {

TextureTransaction::TextureTransaction() {}

TextureTransaction::~TextureTransaction() = default;

bool TextureTransaction::addTexture(const Texture& texture) {
  if (_resultsLatch != nullptr) {
    return false;
  }

  auto texturesAccess = _textures.access();

  return texturesAccess.get().emplace(std::move(texture)).second;
}

bool TextureTransaction::uncompressImages(core::WorkQueue& workqueue) {
  if (_resultsLatch != nullptr) {
    return false;
  }

  auto texturesAccess = _textures.access();
  auto& textures = texturesAccess.get();

  _resultsLatch = core::make_unique<rl::core::Latch>(textures.size());

  for (auto& constTexture : textures) {
    if (constTexture.state() != Texture::State::Compressed) {
      continue;
    }

    /*
     *  Decompressing a texture does not modify its hash or equality result.
     *  Perform a const cast so we can proceed with the decompression.
     */

    auto& texture = const_cast<Texture&>(constTexture);

    workqueue.dispatch([&]() {
      texture.uncompress();
      _resultsLatch->countDown();
    });
  }

  return true;
}

bool TextureTransaction::uploadImagesToVRAM() {
  if (_resultsLatch == nullptr) {
    return false;
  }

  /*
   *  Make sure all the workqueue work items are done processing.
   */
  _resultsLatch->wait();

  /*
   *  Upload to VRAM on the callers thread. Make sure the textures mutex is
   *  acquired after results are done coming in.
   */

  auto texturesAccess = _textures.access();
  auto& textures = texturesAccess.get();

  for (auto& constTexture : textures) {
    /*
     *  Uploading a texture does not modify its hash or equality result.
     *  Perform a const cast so we can proceed with the upload.
     */
    auto& texture = const_cast<Texture&>(constTexture);
    texture.uploadToVRAM();
  }

  return true;
}

bool TextureTransaction::commit(core::WorkQueue& workqueue) {
  RL_RETURN_IF_FALSE(uncompressImages(workqueue));
  RL_RETURN_IF_FALSE(uploadImagesToVRAM());
  return true;
}

}  // namespace compositor
}  // namespace rl
