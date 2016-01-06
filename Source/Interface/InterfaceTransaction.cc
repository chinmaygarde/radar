// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Coordinator/TransactionPayload.h>
#include <Interface/InterfaceTransaction.h>

namespace rl {
namespace interface {

InterfaceTransaction::InterfaceTransaction(const Action&& action)
    : _action(action) {}

void InterfaceTransaction::mark(const Entity& entity,
                                Entity::Property property,
                                Identifier other) {
  auto& transferEntity = _entities[entity.identifier()];

  if (!transferEntity) {
    auto transfer =
        core::make_unique<coordinator::TransferEntity>(entity.identifier());
    transferEntity.swap(transfer);
  }

  transferEntity->record(entity, property, other);
}

void InterfaceTransaction::mark(
    const std::vector<layout::Constraint>& constraints) {
  for (const auto& constraint : constraints) {
    _constraints.emplace_back(constraint);
  }
}

void InterfaceTransaction::mark(
    const std::vector<layout::Suggestion>& suggestions) {
  for (const auto& suggestion : suggestions) {
    _suggestions.emplace_back(suggestion);
  }
}

bool InterfaceTransaction::commit(core::Message& arena) {
  coordinator::TransactionPayload payload(
      std::move(_action), std::move(_entities), std::move(_constraints),
      std::move(_suggestions));
  return arena.encode(payload);
}

}  // namespace interface
}  // namespace rl
