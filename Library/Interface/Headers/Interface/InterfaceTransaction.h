/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Animation/Action.h>
#include <Coordinator/TransactionPayload.h>
#include <Core/Macros.h>
#include <Entity/Entity.h>
#include <Layout/Constraint.h>
#include <Layout/Suggestion.h>
#include <map>

namespace rl {
namespace interface {

class InterfaceTransaction {
 public:
  InterfaceTransaction(const animation::Action&& action);

  void mark(const entity::Entity& entity,
            entity::Entity::Property property,
            core::Name otherIdentifier);

  void mark(const std::vector<layout::Constraint>& constraints);

  void mark(const std::vector<layout::Suggestion>& suggestions);

  void mark(coordinator::TransactionPayload&& payload);

  bool commit(core::Message& arena, std::unique_ptr<core::Archive>& archive);

 private:
  animation::Action _action;
  coordinator::TransactionPayload::EntityMap _entities;
  std::vector<layout::Constraint> _constraints;
  std::vector<layout::Suggestion> _suggestions;
  std::vector<coordinator::TransactionPayload> _extraPayloads;

  RL_DISALLOW_COPY_AND_ASSIGN(InterfaceTransaction);
};

}  // namespace interface
}  // namespace rl
