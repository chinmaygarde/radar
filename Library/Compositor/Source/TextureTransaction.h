// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_COMPOSITOR_TEXTURETRANSACTION_H_
#define RADAR_COMPOSITOR_TEXTURETRANSACTION_H_

#include <Core/Macros.h>
#include <Core/WorkQueue.h>

#include <Image/Image.h>

#include "TextureCache.h"

namespace rl {
namespace compositor {

class TextureTransaction {
 public:
  TextureTransaction();

  ~TextureTransaction();

  RL_WARN_UNUSED_RESULT
  std::shared_ptr<Texture> registerTexture(std::shared_ptr<Texture> texture);

  bool commit(core::WorkQueue* workqueue);

 private:
  TextureCache _textureCache;
  std::vector<std::shared_ptr<Texture>> _textures;
  std::unique_ptr<core::Latch> _latch;

  bool uncompressImages(core::WorkQueue* workqueue);
  bool uploadImagesToVRAM();

  RL_DISALLOW_COPY_AND_ASSIGN(TextureTransaction);
};

}  // namespace compositor
}  // namespace rl

#endif  // RADAR_COMPOSITOR_TEXTURETRANSACTION_H_
