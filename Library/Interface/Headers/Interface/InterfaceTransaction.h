// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_INTERFACE_INTERFACETRANSACTION_
#define RADARLOVE_INTERFACE_INTERFACETRANSACTION_

#include <Core/Core.h>
#include <Animation/Action.h>
#include <Coordinator/EntityArena.h>
#include <Coordinator/TransactionPayload.h>
#include <Entity/Entity.h>
#include <Layout/Constraint.h>
#include <Layout/Suggestion.h>

#include <map>

namespace rl {
namespace interface {

class InterfaceTransaction {
 public:
  explicit InterfaceTransaction(const Action&& action);

  void mark(const Entity& entity,
            Entity::Property property,
            core::Name identifier);

  void mark(const std::vector<layout::Constraint>& constraints);

  void mark(const std::vector<layout::Suggestion>& suggestions);

  void mark(coordinator::TransactionPayload&& payload);

  bool commit(core::Message& arena, std::unique_ptr<core::Archive>& archive);

 private:
  Action _action;
  coordinator::TransactionPayload::EntityMap _entities;
  std::vector<layout::Constraint> _constraints;
  std::vector<layout::Suggestion> _suggestions;
  std::vector<coordinator::TransactionPayload> _extraPayloads;

  RL_DISALLOW_COPY_AND_ASSIGN(InterfaceTransaction);
};

}  // namespace interface
}  // namespace rl

#endif  // RADARLOVE_INTERFACE_INTERFACETRANSACTION_
