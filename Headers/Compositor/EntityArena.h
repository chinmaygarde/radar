// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_COMPOSITOR_ENTITYARENA__
#define __RADARLOVE_COMPOSITOR_ENTITYARENA__

#include <Core/Core.h>
#include <Compositor/PresentationEntity.h>

namespace rl {

class EntityArena {
 public:
  EntityArena(uint8_t* base, size_t maxSize, bool reader);

  PresentationEntity* emplacePresentationEntity(const Entity& entity);
  PresentationEntity* acquireEmplacedEntity();

  size_t encodedEntities() const;

 private:
  uint8_t* _base;
  size_t _maxSize;
  size_t _encodedEntities;
  size_t _decodedEntities;
  size_t _utilization;

  void* alloc(size_t bytes);
  void dealloc(void* allocation);
};

}  // namespace rl

#endif /* defined(__RADARLOVE_COMPOSITOR_ENTITYARENA__) */
