// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Interface/InterfaceTransaction.h>

namespace rl {

InterfaceTransaction::InterfaceTransaction() : _updates() {
}

void InterfaceTransaction::mark(Entity& entity, Entity::Property property) {
  _updates[&entity] |= property;
}

void InterfaceTransaction::commit(EntityArena& arena) {
  for (const auto& pair : _updates) {
    arena.emplaceEntity(*pair.first, MatrixIdentity);
  }
}

}  // namespace rl
