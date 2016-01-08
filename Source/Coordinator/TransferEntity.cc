// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Coordinator/TransferEntity.h>

namespace rl {
namespace coordinator {

TransferEntity::TransferEntity(core::Name identifier)
    : Entity(identifier, false), _updateMask(0) {}

TransferEntity::TransferEntity(const TransferEntity& transferEntity)
    : Entity(transferEntity), _updateMask(transferEntity._updateMask) {}

void TransferEntity::record(const Entity& entity,
                            Entity::Property property,
                            core::Name other) {
  /*
   *  Some properties are not mergeable and the transfer entity needs to keep
   *  some extra information around till the transaction payload is flushed
   */
  switch (property) {
    case Entity::Property::AddedTo:
      _addedTo = other;
      break;
    case Entity::Property::RemovedFrom:
      _removedFrom = other;
      break;
    case Entity::Property::MakeRoot:
      _makeRoot = other;
      break;
    default:
      break;
  }

  RL_ASSERT(entity.identifier() == identifier());

  _updateMask |= (1 << static_cast<PropertyMask>(property));

  merge(entity);
}

bool TransferEntity::walkEnabledProperties(
    PropertyWalkCallback callback) const {
  const auto limit = static_cast<PropertyMask>(Entity::Property::Sentinel);
  for (auto i = 0; i < limit; i++) {
    if (_updateMask & (1 << i)) {
      if (!callback(static_cast<Entity::Property>(i))) {
        return false;
      }
    }
  }
  return true;
}

bool TransferEntity::serialize(core::Message& message) const {
  using Property = Entity::Property;
  return walkEnabledProperties([&](Property property) {
    switch (property) {
      case Property::AddedTo:
        return message.encode(_addedTo);
      case Property::RemovedFrom:
        return message.encode(_removedFrom);
      case Property::Bounds:
        return message.encode(bounds());
      case Property::Position:
        return message.encode(position());
      case Property::AnchorPoint:
        return message.encode(anchorPoint());
      case Property::Transformation:
        return message.encode(transformation());
      case Property::BackgroundColor:
        return message.encode(backgroundColor());
      case Property::Opacity:
        return message.encode(opacity());
      case Property::MakeRoot:
        return message.encode(identifier());
      default:
        return false;
    }
    return false;
  });
}

bool TransferEntity::deserialize(core::Message& message) {
  return walkEnabledProperties([&](Property property) {
    switch (property) {
      case Property::AddedTo:
        return message.decode(_addedTo);
      case Property::RemovedFrom:
        return message.decode(_removedFrom);
      case Property::Bounds:
        return message.decode(_bounds);
      case Property::Position:
        return message.decode(_position);
      case Property::AnchorPoint:
        return message.decode(_anchorPoint);
      case Property::Transformation:
        return message.decode(_transformation);
      case Property::BackgroundColor:
        return message.decode(_backgroundColor);
      case Property::Opacity:
        return message.decode(_opacity);
      case Property::MakeRoot:
        return message.decode(_makeRoot);
      default:
        return false;
    }
    return false;
  });
}

}  // namespace coordinator
}  // namespace rl
