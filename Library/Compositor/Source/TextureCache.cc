// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "TextureCache.h"

namespace rl {
namespace compositor {

TextureCache::TextureCache() = default;

TextureCache::~TextureCache() = default;

std::shared_ptr<Texture> TextureCache::registerTexture(
    std::shared_ptr<Texture> texture) {
  return texture == nullptr ? nullptr : *(_textures.emplace(texture).first);
}

std::size_t TextureCache::CacheTextureHash::operator()(
    const CacheTexture& key) const {
  return key == nullptr ? 0 : Texture::Hash()(*key);
}

bool TextureCache::CacheTextureEqual::operator()(
    const CacheTexture& lhs,
    const CacheTexture& rhs) const {
  return (lhs == nullptr || rhs == nullptr) ? false
                                            : Texture::Equal()(*lhs, *rhs);
}

}  // namespace compositor
}  // namespace rl
