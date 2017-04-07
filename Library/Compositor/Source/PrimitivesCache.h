// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <Compositor/PresentationEntity.h>
#include <Core/Macros.h>
#include <Core/Utilities.h>
#include <unordered_map>
#include "Primitive/Primitive.h"

namespace rl {
namespace compositor {

class PrimitivesCache {
 public:
  enum class ContentType {
    None,
    SolidColor,
    Image,
  };

  enum class PrimitiveType {
    None,
    Box,
    Path,
  };

  PrimitivesCache();

  ~PrimitivesCache();

  std::shared_ptr<Primitive> acquire(const PresentationEntity& entity,
                                     ContentType contentType,
                                     PrimitiveType primitiveType);

  bool invalidate(ContentType contentType, PrimitiveType primitiveType);

 private:
  struct CacheKey {
    ContentType contentType;
    PrimitiveType primitiveType;

    CacheKey(ContentType pContentType, PrimitiveType pPrimitiveType)
        : contentType(pContentType), primitiveType(pPrimitiveType) {}
  };

  struct CacheKeyHash {
    std::size_t operator()(CacheKey const& key) const {
      std::size_t hash = 0;
      core::HashCombine(hash, static_cast<uint64_t>(key.contentType));
      core::HashCombine(hash, static_cast<uint64_t>(key.primitiveType));
      return hash;
    }
  };

  struct CacheKeyEqual {
    bool operator()(const CacheKey& lhs, const CacheKey& rhs) const {
      return lhs.contentType == rhs.contentType &&
             lhs.primitiveType == rhs.primitiveType;
    }
  };

  using PrimitivesMap = std::unordered_map<CacheKey,
                                           std::shared_ptr<Primitive>,
                                           CacheKeyHash,
                                           CacheKeyEqual>;
  PrimitivesMap _primitivesMap;

  std::shared_ptr<Primitive> createPrimitive(const PresentationEntity& entity,
                                             ContentType contentType,
                                             PrimitiveType primitiveType);
  std::shared_ptr<Primitive> createColoredPrimitive(
      const PresentationEntity& entity,
      PrimitiveType type) const;
  std::shared_ptr<Primitive> createTexturedPrimitive(
      const PresentationEntity& entity,
      PrimitiveType type) const;

  RL_DISALLOW_COPY_AND_ASSIGN(PrimitivesCache);
};

}  // namespace compositor
}  // namespace rl
