// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_COORDINATOR_TRANSFERENTITY_H_
#define RADAR_COORDINATOR_TRANSFERENTITY_H_

#include <Core/Macros.h>
#include <Entity/Entity.h>

namespace rl {
namespace coordinator {

class TransferEntity : public entity::Entity, public core::MessageSerializable {
 public:
  using PropertyWalkCallback = std::function<bool(Property)>;

  TransferEntity(core::Name identifier = core::Name());

  TransferEntity(const TransferEntity& transferEntity);

  void record(const Entity& entity,
              Entity::Property property,
              core::Name other);

  bool walkEnabledProperties(PropertyMaskType extraMask,
                             PropertyWalkCallback callback) const;

  core::Name addedToTarget() const;

  core::Name removedFromTarget() const;

  core::Name makeRootTarget() const;

  bool serialize(core::Message& message) const override;

  bool deserialize(core::Message& message, core::Namespace* ns) override;

  static const core::ArchiveDef ArchiveDefinition;

  ArchiveName archiveName() const override;

  bool serialize(core::ArchiveItem& item) const override;

  bool deserialize(core::ArchiveItem& item, core::Namespace* ns) override;

 private:
  PropertyMaskType _updateMask;

  core::Name _addedTo;
  core::Name _removedFrom;
  core::Name _makeRoot;

  bool serializeProperty(core::Message& message, Property property);
  bool deserializeProperty(core::Message& message, Property property);

  RL_DISALLOW_ASSIGN(TransferEntity);
};

}  // namespace coordinator
}  // namespace rl

#endif  // RADAR_COORDINATOR_TRANSFERENTITY_H_
