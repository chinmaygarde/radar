// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_INTERFACE_INTERFACETRANSACTION__
#define __RADARLOVE_INTERFACE_INTERFACETRANSACTION__

#include <Core/Core.h>
#include <Interface/Entity.h>
#include <Compositor/EntityArena.h>

#include <map>

namespace rl {
class InterfaceTransaction {
 public:
  explicit InterfaceTransaction();

  void mark(const Entity& entity, Entity::Property property);

  void commit(EntityArena& arena);

 private:
  std::map<Entity::Identifier, std::unique_ptr<TransferEntity>> _entities;

  DISALLOW_COPY_AND_ASSIGN(InterfaceTransaction);
};
}

#endif /* defined(__RADARLOVE_INTERFACE_INTERFACETRANSACTION__) */
