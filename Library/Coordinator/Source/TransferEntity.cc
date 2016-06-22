// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Coordinator/TransferEntity.h>

namespace rl {
namespace coordinator {

TransferEntity::TransferEntity(core::Name identifier)
    : Entity(identifier, nullptr), _updateMask(0) {}

TransferEntity::TransferEntity(const TransferEntity& transferEntity)
    : Entity(transferEntity.identifier(), nullptr),
      _updateMask(transferEntity._updateMask),
      _addedTo(transferEntity._addedTo),
      _removedFrom(transferEntity._removedFrom),
      _makeRoot(transferEntity._makeRoot) {
  mergeProperties(transferEntity, ~0);
}

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
  RL_ASSERT(entity.identifier() == identifier());

  auto propertyMask = (1 << static_cast<PropertyMask>(property));
  _updateMask |= propertyMask;

  /*
   *  Some properties are not mergeable and the transfer entity needs to keep
   *  some extra information around till the transaction payload is flushed
   */
  switch (property) {
    case Entity::Property::AddedTo:
      _addedTo = other;
      return;
    case Entity::Property::RemovedFrom:
      _removedFrom = other;
      return;
    case Entity::Property::MakeRoot:
      _makeRoot = other;
      return;
    default:
      break;
  }

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
  RL_RETURN_IF_FALSE(message.encode(_identifier));
  RL_RETURN_IF_FALSE(message.encode(_updateMask));

  using Property = Entity::Property;
  return walkEnabledProperties(~0 /* all */, [&](Property property) {
    switch (property) {
      case Property::AddedTo:
        return message.encode(_addedTo);
      case Property::RemovedFrom:
        return message.encode(_removedFrom);
      case Property::Bounds:
        return message.encode(_bounds);
      case Property::Position:
        return message.encode(_position);
      case Property::AnchorPoint:
        return message.encode(_anchorPoint);
      case Property::Transformation:
        return message.encode(_transformation);
      case Property::BackgroundColor:
        return message.encode(_backgroundColor);
      case Property::Contents:
        return message.encode(_contents);
      case Property::Opacity:
        return message.encode(_opacity);
      case Property::MakeRoot:
        return message.encode(_identifier);
      case Property::None:
      case Property::Sentinel:
        RL_ASSERT(false);
        break;
    }
    return false;
  });
}

bool TransferEntity::deserialize(core::Message& message, core::Namespace* ns) {
  RL_RETURN_IF_FALSE(message.decode(_identifier, ns));
  RL_RETURN_IF_FALSE(message.decode(_updateMask, ns));
  RL_RETURN_IF_FALSE(
      walkEnabledProperties(~0 /* all */, [&](Property property) {
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
          case Property::Contents:
            return message.decode(_contents, ns);
          case Property::Opacity:
            return message.decode(_opacity, ns);
          case Property::MakeRoot:
            return message.decode(_makeRoot, ns);
          case Property::None:
          case Property::Sentinel:
            return false;
        }
        return false;
      }))
  return true;
}

enum ArchiveKey {
  UpdateMask,
  AddedTo,
  RemovedFrom,
  MakeRoot,
};

const core::ArchiveDef TransferEntity::ArchiveDefinition = {
    /* .superClass = */ &Entity::ArchiveDefinition,
    /* .className = */ "TransferEntity",
    /* .autoAssignName = */ false,
    /* .members = */
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
  RL_RETURN_IF_FALSE(item.encodeSuper<Entity>(*this));
  RL_RETURN_IF_FALSE(item.encode(ArchiveKey::UpdateMask, _updateMask));
  RL_RETURN_IF_FALSE(item.encode(ArchiveKey::AddedTo, _addedTo));
  RL_RETURN_IF_FALSE(item.encode(ArchiveKey::RemovedFrom, _removedFrom));
  RL_RETURN_IF_FALSE(item.encode(ArchiveKey::MakeRoot, _makeRoot));

  return true;
}

bool TransferEntity::deserialize(core::ArchiveItem& item, core::Namespace* ns) {
  RL_RETURN_IF_FALSE(item.decodeSuper<Entity>(*this, ns));
  RL_RETURN_IF_FALSE(item.decode(ArchiveKey::UpdateMask, _updateMask));
  RL_RETURN_IF_FALSE(item.decode(ArchiveKey::AddedTo, _addedTo, ns));
  RL_RETURN_IF_FALSE(item.decode(ArchiveKey::RemovedFrom, _removedFrom, ns));
  RL_RETURN_IF_FALSE(item.decode(ArchiveKey::MakeRoot, _makeRoot, ns));

  return true;
}

}  // namespace coordinator
}  // namespace rl
