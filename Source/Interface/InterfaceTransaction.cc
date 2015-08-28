// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Interface/InterfaceTransaction.h>

namespace rl {

InterfaceTransaction::InterfaceTransaction() : _entities() {
}

void InterfaceTransaction::mark(const Entity& entity,
                                Entity::Property property) {
  auto& transferEntity = _entities[entity.identifier()];

  if (!transferEntity) {
    auto transfer = rl::make_unique<TransferEntity>(entity.identifier());
    transferEntity.swap(transfer);
  }

  transferEntity->record(entity, property);
}

bool InterfaceTransaction::commit(Message& arena) {
  bool result = false;

  for (const auto& pair : _entities) {
    result |= (*(pair.second)).serialize(arena);
  }

  return result;
}

}  // namespace rl
