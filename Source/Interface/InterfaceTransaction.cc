// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Interface/InterfaceTransaction.h>
#include <Coordinator/TransactionPayload.h>

namespace rl {
namespace interface {

InterfaceTransaction::InterfaceTransaction(const Action&& action)
    : _action(action) {
}

void InterfaceTransaction::mark(const Entity& entity,
                                Entity::Property property,
                                Entity::Identifier other) {
  auto& transferEntity = _entities[entity.identifier()];

  if (!transferEntity) {
    auto transfer = core::make_unique<TransferEntity>(entity.identifier());
    transferEntity.swap(transfer);
  }

  transferEntity->record(entity, property, other);
}

void InterfaceTransaction::mark(recognition::GestureRecognizer&& recognizer) {
  _recognizers.emplace_back(std::move(recognizer));
}

bool InterfaceTransaction::commit(core::Message& arena) {
  TransactionPayload payload(std::move(_action), std::move(_entities),
                             std::move(_recognizers));
  return payload.serialize(arena);
}

}  // namespace interface
}  // namespace rl
