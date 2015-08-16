// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/EntityArena.h>

namespace rl {

struct EntityArenaHeader {
  size_t entityCount;
};

#define WRITE_ENTITY_COUNT(x) \
  reinterpret_cast<EntityArenaHeader*>(_base)->entityCount = (x);

EntityArena::EntityArena(uint8_t* base, size_t maxSize, bool reader)
    : _maxSize(maxSize), _base(base), _utilization(sizeof(EntityArenaHeader)) {
  if (base == nullptr) {
    _encodedEntities = 0;
    return;
  }

  if (reader) {
    auto header = reinterpret_cast<EntityArenaHeader*>(base);
    _encodedEntities = header->entityCount;
  } else {
    _encodedEntities = 0;
    WRITE_ENTITY_COUNT(0);
  }
}

PresentationEntity* EntityArena::emplaceEntity(const Entity& entity,
                                               int32_t parentIndex) {
  auto allocation = alloc(sizeof(PresentationEntity));

  if (allocation == nullptr) {
    return nullptr;
  }

  WRITE_ENTITY_COUNT(++_encodedEntities);

  return (new (allocation) PresentationEntity(entity, parentIndex));
}

const PresentationEntity& EntityArena::operator[](size_t index) const {
  RL_ASSERT(index < _encodedEntities);
  auto allocation =
      _base + sizeof(EntityArenaHeader) + index * sizeof(PresentationEntity);
  return *reinterpret_cast<PresentationEntity*>(allocation);
}

size_t EntityArena::encodedEntities() const {
  return _encodedEntities;
}

void* EntityArena::alloc(size_t bytes) {
  if (_utilization + bytes > _maxSize) {
    return nullptr;
  }

  auto allocated = _base + _utilization;
  _utilization += bytes;
  return allocated;
}

size_t EntityArena::Size(size_t entityCount) {
  return sizeof(EntityArenaHeader) + entityCount * sizeof(PresentationEntity);
}

}  // namespace rl
