// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_INTERFACE_INTERFACETRANSACTION_
#define RADARLOVE_INTERFACE_INTERFACETRANSACTION_

#include <Core/Core.h>
#include <Interface/Action.h>
#include <Interface/Entity.h>
#include <Coordinator/EntityArena.h>
#include <Recognition/GestureRecognizer.h>

#include <map>

namespace rl {
namespace interface {

class InterfaceTransaction {
 public:
  explicit InterfaceTransaction(const Action&& action);

  void mark(const Entity& entity,
            Entity::Property property,
            Entity::Identifier);

  void mark(recognition::GestureRecognizer&& recognizer);

  bool commit(core::Message& arena);

 private:
  Action _action;
  std::map<Entity::Identifier, std::unique_ptr<TransferEntity>> _entities;
  std::vector<recognition::GestureRecognizer> _recognizers;

  RL_DISALLOW_COPY_AND_ASSIGN(InterfaceTransaction);
};

}  // namespace interface
}  // namespace rl

#endif  // RADARLOVE_INTERFACE_INTERFACETRANSACTION_
