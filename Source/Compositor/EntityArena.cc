// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/EntityArena.h>

namespace rl {

struct EntityArenaHeader {
  size_t entityCount;
};

EntityArena::EntityArena(uint8_t* base, size_t maxSize, bool reader)
    : _maxSize(maxSize),
      _base(base),
      _decodedEntities(0),
      _utilization(sizeof(EntityArenaHeader)) {
  assert(maxSize > sizeof(EntityArenaHeader));

  if (reader) {
    auto header = reinterpret_cast<EntityArenaHeader*>(base);
    _encodedEntities = header->entityCount;
  } else {
    _encodedEntities = 0;
  }
}

PresentationEntity* EntityArena::emplacePresentationEntity(
    const Entity& entity) {
  auto allocation = alloc(sizeof(PresentationEntity));

  if (allocation == nullptr) {
    return nullptr;
  }

  reinterpret_cast<EntityArenaHeader*>(_base)->entityCount = ++_encodedEntities;

  return (new (allocation) PresentationEntity(entity));
}

size_t EntityArena::encodedEntities() const {
  return _encodedEntities;
}

PresentationEntity* EntityArena::acquireEmplacedEntity() {
  if (_decodedEntities == _encodedEntities) {
    return nullptr;
  }

  _decodedEntities++;

  return (
      reinterpret_cast<PresentationEntity*>(alloc(sizeof(PresentationEntity))));
}

void* EntityArena::alloc(size_t bytes) {
  if (_utilization + bytes > _maxSize) {
    return nullptr;
  }

  auto allocated = _base + _utilization;
  _utilization += bytes;
  return allocated;
}

}  // namespace rl
