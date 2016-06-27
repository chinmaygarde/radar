// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "TextureTransaction.h"

namespace rl {
namespace compositor {

TextureTransaction::TextureTransaction() {}

TextureTransaction::~TextureTransaction() = default;

bool TextureTransaction::addImage(image::Image image) {
  if (_resultsLatch != nullptr) {
    return false;
  }

  auto imagesAccess = _images.access();
  auto& images = imagesAccess.get();

  return images.emplace(std::move(image)).second;
}

bool TextureTransaction::uncompressImages(core::WorkQueue& workqueue) {
  if (_resultsLatch != nullptr) {
    return false;
  }

  auto imagesAccess = _images.access();
  auto& images = imagesAccess.get();

  _resultsLatch = core::make_unique<rl::core::Latch>(images.size());

  for (auto image : images) {
    workqueue.dispatch(
        std::bind(&TextureTransaction::uncompressOnWQ, this, std::move(image)));
  }

  images.clear();

  return true;
}

void TextureTransaction::uncompressOnWQ(image::Image image) {
  Texture texture(image);

  /*
   *  Attempt to uncompress the image but only acquire the lock on the results
   *  once (and if) the operation was successful.
   */
  if (texture.uncompress() && texture.state() == Texture::State::Uncompressed) {
    auto resultsAccess = _results.access();
    auto& results = resultsAccess.get();
    results.emplace(image, std::move(texture));
  }

  /*
   *  Successful decompression or not, we have to count down on the latch.
   */
  RL_ASSERT(_resultsLatch != nullptr);
  _resultsLatch->countDown();
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
   *  Upload to VRAM on the callers thread. Make sure the results mutex is
   *  acquired after results are done coming in.
   */

  auto resultsAccess = _results.access();
  auto& results = resultsAccess.get();

  for (auto& result : results) {
    result.second.uploadToVRAM();
  }

  return true;
}

}  // namespace compositor
}  // namespace rl
