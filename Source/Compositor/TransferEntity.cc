// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/TransferEntity.h>

namespace rl {

TransferEntity::TransferEntity(Identifier identifier)
    : Entity(identifier), _updateMask(0) {
}

TransferEntity::TransferEntity(const TransferEntity& transferEntity)
    : Entity(transferEntity), _updateMask(transferEntity._updateMask) {
}

uint64_t TransferEntity::updateMask() const {
  return _updateMask;
}

void TransferEntity::record(const Entity& entity, Entity::Property property) {
  RL_ASSERT(entity.identifier() == identifier());
  RL_ASSERT(property != 0);

  _updateMask |= property;

  /*
   *  TODO: Avoid a full merge
   */
  merge(entity);
}

}  // namespace rl
