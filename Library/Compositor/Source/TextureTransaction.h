// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_COMPOSITOR_TEXTURETRANSACTION_H_
#define RADAR_COMPOSITOR_TEXTURETRANSACTION_H_

#include <Core/Macros.h>
#include <Core/WorkQueue.h>
#include <Core/Mutexed.h>

#include <Image/Image.h>

#include "Texture.h"

#include <unordered_map>

namespace rl {
namespace compositor {

class TextureTransaction {
 public:
  TextureTransaction();

  ~TextureTransaction();

  bool addTexture(const Texture& texture);

  bool commit(core::WorkQueue& workqueue);

 private:
  using Textures =
      core::Mutexed<std::unordered_set<Texture, Texture::Hash, Texture::Equal>>;

  Textures _textures;

  std::unique_ptr<core::Latch> _resultsLatch;

  bool uncompressImages(core::WorkQueue& workqueue);
  bool uploadImagesToVRAM();

  RL_DISALLOW_COPY_AND_ASSIGN(TextureTransaction);
};

}  // namespace compositor
}  // namespace rl

#endif  // RADAR_COMPOSITOR_TEXTURETRANSACTION_H_
