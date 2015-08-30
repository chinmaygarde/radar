// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Interface/InterfaceTransaction.h>

namespace rl {

InterfaceTransaction::InterfaceTransaction(const Action&& action)
    : _entities(), _action(action) {
}

void InterfaceTransaction::mark(const Entity& entity,
                                Entity::Property property,
                                Entity::Identifier other) {
  auto& transferEntity = _entities[entity.identifier()];

  if (!transferEntity) {
    auto transfer = rl::make_unique<TransferEntity>(entity.identifier());
    transferEntity.swap(transfer);
  }

  transferEntity->record(entity, property, other);
}

bool InterfaceTransaction::commit(Message& arena) {
  bool result = true;

  /*
   *  Step 1: Encode the Action
   */

  result &= _action.serialize(arena);

  /*
   *  Step 2: Encode the transfer record count
   *          This is a bit weird since we dont know how many records the entity
   *          will decide to encode. So we just allocate space for the count and
   *          come back to it later.
   */
  auto countOffset = arena.encodeRawOffsetUnsafe(sizeof(size_t));
  result &= (countOffset != std::numeric_limits<size_t>::max());

  if (!result) {
    return false;
  }

  /*
   *  Step 3: Encode the transfer records
   */
  size_t transferRecordsEncoded = 0;
  for (const auto& pair : _entities) {
    transferRecordsEncoded += (*(pair.second)).serialize(arena);
  }

  /*
   *  Step 2.1: See step 2
   */
  if (auto count = reinterpret_cast<size_t*>(arena[countOffset])) {
    memcpy(count, &transferRecordsEncoded, sizeof(size_t));
  } else {
    return false;
  }

  return result;
}

}  // namespace rl
