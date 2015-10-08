// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_COMPOSITOR_TRANSFERENTITY_H_
#define RADARLOVE_COMPOSITOR_TRANSFERENTITY_H_

#include <Core/Core.h>
#include <Interface/Entity.h>

namespace rl {

class TransferEntity : public Entity {
 public:
  explicit TransferEntity(Identifier identifier);
  explicit TransferEntity(const TransferEntity& transferEntity);

  void record(const Entity& entity,
              Entity::Property property,
              Entity::Identifier other);

  size_t serialize(Message& message);

 private:
  uint64_t _updateMask;
  bool _lastHierarchyUpdateWasAdd;
  Identifier _firstRemovedFrom;
  Identifier _lastAddedTo;

  RL_DISALLOW_ASSIGN(TransferEntity);
};

}  // namespace rl

#endif  // RADARLOVE_COMPOSITOR_TRANSFERENTITY_H_
