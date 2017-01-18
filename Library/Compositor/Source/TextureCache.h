// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_COMPOSITOR_TEXTURECACHE_H_
#define RADAR_COMPOSITOR_TEXTURECACHE_H_

#include <Core/Macros.h>
#include <unordered_set>
#include "Texture.h"

namespace rl {
namespace compositor {

class TextureCache {
 public:
  TextureCache();

  ~TextureCache();

  RL_WARN_UNUSED_RESULT
  std::shared_ptr<Texture> registerTexture(std::shared_ptr<Texture> texture);

 private:
  using CacheTexture = std::shared_ptr<Texture>;

  struct CacheTextureHash {
    std::size_t operator()(const CacheTexture& key) const;
  };

  struct CacheTextureEqual {
    bool operator()(const CacheTexture& lhs, const CacheTexture& rhs) const;
  };

  using Textures =
      std::unordered_set<CacheTexture, CacheTextureHash, CacheTextureEqual>;

  Textures _textures;

  RL_DISALLOW_COPY_AND_ASSIGN(TextureCache);
};

}  // namespace compositor
}  // namespace rl

#endif  // RADAR_COMPOSITOR_TEXTURECACHE_H_
