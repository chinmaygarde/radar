// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/TransferEntity.h>
#include <Compositor/TransferRecord.h>

namespace rl {

TransferEntity::TransferEntity(Identifier identifier)
    : Entity(identifier),
      _updateMask(0),
      _lastHierarchyUpdateWasAdd(false),
      _firstRemovedFrom(IdentifierNone),
      _lastAddedTo(IdentifierNone) {
}

TransferEntity::TransferEntity(const TransferEntity& transferEntity)
    : Entity(transferEntity), _updateMask(transferEntity._updateMask) {
}

void TransferEntity::record(const Entity& entity,
                            Entity::Property property,
                            Entity::Identifier other) {
  RL_ASSERT(entity.identifier() == identifier());

  _updateMask |= property;

  if (property & Hierarchy) {
    if (property & AddedTo) {
      _lastHierarchyUpdateWasAdd = true;
      _lastAddedTo = other;
    } else {
      _lastHierarchyUpdateWasAdd = false;
      if (_firstRemovedFrom == IdentifierNone) {
        _firstRemovedFrom = other;
      }
    }
  }

  merge(entity);
}

size_t TransferEntity::serialize(Message& message) {
  size_t encodedRecords = 0;

  const auto mask = _updateMask;

#define SerializeProperty(prop, func)                                       \
  if (mask & prop) {                                                        \
    encodedRecords +=                                                       \
        TransferRecord::EmplaceInMessage(message, identifier(), prop, func) \
            ? 1                                                             \
            : 0;                                                            \
  }

  SerializeProperty(Created, identifier());

  SerializeProperty(Bounds, bounds());
  SerializeProperty(Position, position());
  SerializeProperty(AnchorPoint, anchorPoint());
  SerializeProperty(Transformation, transformation());
  SerializeProperty(BackgroundColor, backgroundColor());
  SerializeProperty(Opacity, opacity());
  SerializeProperty(MakeRoot, identifier());

  if (_lastHierarchyUpdateWasAdd) {
    SerializeProperty(RemovedFrom, _firstRemovedFrom);
    SerializeProperty(AddedTo, _lastAddedTo);
  } else {
    SerializeProperty(AddedTo, _lastAddedTo);
    SerializeProperty(RemovedFrom, _firstRemovedFrom);
  }

  SerializeProperty(Destroyed, identifier());

#undef SerializeProperty

  return encodedRecords;
}

}  // namespace rl
