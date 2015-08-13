// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/EntityArena.h>

namespace rl {

EntityArena::EntityArena(uint8_t* base, size_t maxSize)
    : _maxSize(maxSize), _base(base), _utilization(0) {
}

PresentationEntity* EntityArena::emplacePresentationEntity(
    const Entity& entity) {
  auto allocation = alloc(sizeof(PresentationEntity));

  if (allocation == nullptr) {
    return nullptr;
  }

  return (new (allocation) PresentationEntity(entity));
}

PresentationEntity* EntityArena::acquireEmplacedEntity() {
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
