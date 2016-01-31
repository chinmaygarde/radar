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

core::Name TransferEntity::addedToTarget() const {
  return _addedTo;
}

core::Name TransferEntity::removedFromTarget() const {
  return _removedFrom;
}

core::Name TransferEntity::makeRootTarget() const {
  return _makeRoot;
}

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

  auto propertyMask = (1 << static_cast<PropertyMask>(property));
  _updateMask |= propertyMask;

  /*
   *  Make a note of the updated property so that we can send the new value
   *  on transaction flush
   */
  mergeProperties(entity, propertyMask);
}

bool TransferEntity::walkEnabledProperties(
    PropertyMaskType extraMask,
    PropertyWalkCallback callback) const {
  const auto limit = static_cast<PropertyMaskType>(Entity::Property::Sentinel);

  const auto mask = extraMask & _updateMask;

  if (mask == 0) {
    return true;
  }

  for (auto i = 0; i < limit; i++) {
    if (mask & (1 << i)) {
      if (!callback(static_cast<Entity::Property>(i))) {
        return false;
      }
    }
  }

  return true;
}

bool TransferEntity::serialize(core::Message& message) const {
  auto success = message.encode(_identifier);
  success &= message.encode(_updateMask);

  if (!success) {
    return false;
  }

  using Property = Entity::Property;
  return walkEnabledProperties(~0 /* all */, [&](Property property) {
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

bool TransferEntity::deserialize(core::Message& message, core::Namespace* ns) {
  auto success = message.decode(_identifier, ns);
  success &= message.decode(_updateMask, ns);

  if (!success) {
    return false;
  }

  return walkEnabledProperties(~0 /* all */, [&](Property property) {
    switch (property) {
      case Property::AddedTo:
        return message.decode(_addedTo, ns);
      case Property::RemovedFrom:
        return message.decode(_removedFrom, ns);
      case Property::Bounds:
        return message.decode(_bounds, ns);
      case Property::Position:
        return message.decode(_position, ns);
      case Property::AnchorPoint:
        return message.decode(_anchorPoint, ns);
      case Property::Transformation:
        return message.decode(_transformation, ns);
      case Property::BackgroundColor:
        return message.decode(_backgroundColor, ns);
      case Property::Opacity:
        return message.decode(_opacity, ns);
      case Property::MakeRoot:
        return message.decode(_makeRoot, ns);
      default:
        return false;
    }
    return false;
  });
}

enum ArchiveKey {
  UpdateMask,
  AddedTo,
  RemovedFrom,
  MakeRoot,
};

const core::ArchiveDef TransferEntity::ArchiveDefinition = {
    .superClass = &Entity::ArchiveDefinition,
    .className = "TransferEntity",
    .autoAssignName = false,
    .members =
        {
            ArchiveKey::UpdateMask,   //
            ArchiveKey::AddedTo,      //
            ArchiveKey::RemovedFrom,  //
            ArchiveKey::MakeRoot,     //
        },
};

TransferEntity::ArchiveName TransferEntity::archiveName() const {
  return *_identifier.handle();
}

bool TransferEntity::serialize(core::ArchiveItem& item) const {
  auto result = true;
  result &= item.encodeSuper<Entity>(*this);
  result &= item.encode(ArchiveKey::UpdateMask, _updateMask);
  return result;
}

bool TransferEntity::deserialize(core::ArchiveItem& item, core::Namespace* ns) {
  auto result = true;
  result &= item.decode(ArchiveKey::UpdateMask, _updateMask);
  return result;
}

}  // namespace coordinator
}  // namespace rl
