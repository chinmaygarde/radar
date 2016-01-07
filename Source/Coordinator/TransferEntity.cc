// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Coordinator/TransferEntity.h>
#include <Coordinator/TransferRecord.h>

namespace rl {
namespace coordinator {

TransferEntity::TransferEntity(interface::Identifier identifier)
    : Entity(identifier, false),
      _updateMask(0),
      _lastHierarchyUpdateWasAdd(false),
      _firstRemovedFrom(interface::IdentifierNone),
      _lastAddedTo(interface::IdentifierNone) {}

TransferEntity::TransferEntity(const TransferEntity& transferEntity)
    : Entity(transferEntity),
      _updateMask(transferEntity._updateMask),
      _firstRemovedFrom(interface::IdentifierNone),
      _lastAddedTo(interface::IdentifierNone) {}

void TransferEntity::record(const Entity& entity,
                            Entity::Property property,
                            interface::Identifier other) {
  RL_ASSERT(entity.identifier() == identifier());

  _updateMask |= property;

  if (property & Hierarchy) {
    if (property & AddedTo) {
      _lastHierarchyUpdateWasAdd = true;
      _lastAddedTo = other;
    } else {
      _lastHierarchyUpdateWasAdd = false;
      if (_firstRemovedFrom == interface::IdentifierNone) {
        _firstRemovedFrom = other;
      }
    }
  }

  merge(entity);
}

size_t TransferEntity::serialize(core::Message& message) {
  size_t encodedRecords = 0;

  const auto mask = _updateMask;

#define SerializeProperty(prop, func)                                       \
  if (mask & prop) {                                                        \
    encodedRecords +=                                                       \
        TransferRecord::EmplaceInMessage(message, identifier(), prop, func) \
            ? 1                                                             \
            : 0;                                                            \
  }

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

#undef SerializeProperty

  return encodedRecords;
}

}  // namespace coordinator
}  // namespace rl
