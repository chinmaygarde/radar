// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Coordinator/TransactionPayload.h>
#include <Interface/InterfaceTransaction.h>

namespace rl {
namespace interface {

InterfaceTransaction::InterfaceTransaction(const animation::Action&& action)
    : _action(action) {}

void InterfaceTransaction::mark(const entity::Entity& entity,
                                entity::Entity::Property property,
                                core::Name other) {
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

void InterfaceTransaction::mark(coordinator::TransactionPayload&& payload) {
  _extraPayloads.emplace_back(std::move(payload));
}

bool InterfaceTransaction::commit(core::Message& arena,
                                  std::unique_ptr<core::Archive>& archive) {
  coordinator::TransactionPayload payload(
      std::move(_action), std::move(_entities), std::move(_constraints),
      std::move(_suggestions));

  /*
   *  Write to the message arena
   */
  auto result = arena.encode(payload);
  for (const auto& extraPayload : _extraPayloads) {
    result &= arena.encode(extraPayload);
  }

  /*
   *  Write to the archive if one is available
   */
  if (archive) {
    result &= archive->archive(payload);
    for (const auto& extraPayload : _extraPayloads) {
      result &= archive->archive(extraPayload);
    }
  }

  return result;
}

}  // namespace interface
}  // namespace rl
