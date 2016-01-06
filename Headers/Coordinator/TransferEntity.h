// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_COORDINATOR_TRANSFERENTITY_H_
#define RADARLOVE_COORDINATOR_TRANSFERENTITY_H_

#include <Core/Core.h>
#include <Interface/Entity.h>

namespace rl {
namespace coordinator {

class TransferEntity : public interface::Entity {
 public:
  explicit TransferEntity(interface::Identifier identifier);
  explicit TransferEntity(const TransferEntity& transferEntity);

  void record(const Entity& entity,
              Entity::Property property,
              interface::Identifier other);

  size_t serialize(core::Message& message);

 private:
  uint64_t _updateMask;
  bool _lastHierarchyUpdateWasAdd;
  interface::Identifier _firstRemovedFrom;
  interface::Identifier _lastAddedTo;

  RL_DISALLOW_ASSIGN(TransferEntity);
};

}  // namespace coordinator
}  // namespace rl

#endif  // RADARLOVE_COORDINATOR_TRANSFERENTITY_H_
