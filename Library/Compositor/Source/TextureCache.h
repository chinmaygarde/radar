/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

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
