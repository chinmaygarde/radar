// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_COMPOSITOR_ENTITYARENA_
#define RADARLOVE_COMPOSITOR_ENTITYARENA_

#include <Core/Core.h>
#include <Coordinator/TransferEntity.h>
#include <Geometry/Geometry.h>

namespace rl {

class EntityArena {
 public:
  explicit EntityArena(uint8_t* base, size_t maxSize, bool reader);

  TransferEntity& emplaceTransferEntity(const TransferEntity& entity);

  const TransferEntity& operator[](size_t index) const;

  size_t encodedEntities() const;

  /**
   *  Compute the size of the allocation required to serevice the given number
   *  of entities and all the necessary book-keeping information about the same.
   *
   *  @param entityCount the number of entities to service
   *
   *  @return the size of the arena required to service the given
   *          number of entities
   */
  static size_t Size(size_t entityCount);

 private:
  uint8_t* _base;
  size_t _maxSize;
  size_t _encodedEntities;
  size_t _utilization;

  void* alloc(size_t bytes);

  RL_DISALLOW_COPY_AND_ASSIGN(EntityArena);
};

}  // namespace rl

#endif  // RADARLOVE_COMPOSITOR_ENTITYARENA_
