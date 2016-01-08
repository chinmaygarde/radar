// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_COORDINATOR_TRANSFERENTITY_H_
#define RADARLOVE_COORDINATOR_TRANSFERENTITY_H_

#include <Core/Core.h>
#include <Interface/Entity.h>

namespace rl {
namespace coordinator {

class TransferEntity : public interface::Entity, public core::Serializable {
 public:
  explicit TransferEntity(core::Name identifier);
  explicit TransferEntity(const TransferEntity& transferEntity);

  void record(const Entity& entity,
              Entity::Property property,
              core::Name other);

  bool serialize(core::Message& message) const override;

  bool deserialize(core::Message& message) override;

 private:
  PropertyMask _updateMask;

  core::Name _addedTo;
  core::Name _removedFrom;
  core::Name _makeRoot;

  bool serializeProperty(core::Message& message, Property property);
  bool deserializeProperty(core::Message& message, Property property);

  using PropertyWalkCallback = std::function<bool(Property)>;
  bool walkEnabledProperties(PropertyWalkCallback callback) const;

  RL_DISALLOW_ASSIGN(TransferEntity);
};

}  // namespace coordinator
}  // namespace rl

#endif  // RADARLOVE_COORDINATOR_TRANSFERENTITY_H_
