// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_INTERFACE_INTERFACETRANSACTION__
#define __RADARLOVE_INTERFACE_INTERFACETRANSACTION__

#include <Core/Core.h>
#include <Interface/Action.h>
#include <Interface/Entity.h>
#include <Compositor/EntityArena.h>
#include <Recognition/GestureRecognizer.h>

#include <map>

namespace rl {

class GestureRecognizer;

class InterfaceTransaction {
 public:
  explicit InterfaceTransaction(const Action&& action);

  void mark(const Entity& entity,
            Entity::Property property,
            Entity::Identifier);

  void mark(GestureRecognizer&& recognizer);

  bool commit(Message& arena);

 private:
  Action _action;
  std::map<Entity::Identifier, std::unique_ptr<TransferEntity>> _entities;
  std::vector<GestureRecognizer> _recognizers;

  RL_DISALLOW_COPY_AND_ASSIGN(InterfaceTransaction);
};
}

#endif /* defined(__RADARLOVE_INTERFACE_INTERFACETRANSACTION__) */
