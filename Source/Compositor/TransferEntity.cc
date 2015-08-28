// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/TransferEntity.h>
#include <Compositor/TransferRecord.h>

namespace rl {

TransferEntity::TransferEntity(Identifier identifier)
    : Entity(identifier), _updateMask(0) {
}

TransferEntity::TransferEntity(const TransferEntity& transferEntity)
    : Entity(transferEntity), _updateMask(transferEntity._updateMask) {
}

void TransferEntity::record(const Entity& entity, Entity::Property property) {
  RL_ASSERT(entity.identifier() == identifier());

  _updateMask |= property;
  merge(entity);
}

bool TransferEntity::serialize(Message& message) {
  bool success = false;

#define SerializeProperty(prop, func)                                       \
  if (_updateMask & prop) {                                                 \
    success |= TransferRecord::Emplaced(message, identifier(), prop, func); \
  }

  SerializeProperty(Created, identifier());
  SerializeProperty(Bounds, bounds());
  SerializeProperty(Position, position());
  SerializeProperty(AnchorPoint, anchorPoint());
  SerializeProperty(Transformation, transformation());
  SerializeProperty(BackgroundColor, backgroundColor());
  SerializeProperty(Opacity, opacity());
  SerializeProperty(Destroyed, identifier());

#if 0
  SerializeProperty(AddedTo, );
  SerializeProperty(RemovedFrom, );
#endif

#undef SerializeProperty

  return success;
}

}  // namespace rl
