// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/Patch.h>
#include <Interface/Layer.h>

namespace rl {

Patch::Patch() : _marks() {
}

void Patch::mark(const Layer& layer, PatchChunk::Command command) {
  _marks[layer.patchIdentifier()].insert(command);
}

Message Patch::flatten() const {
  Message m;
  assert(false);
  return m;
}

}  // namespace rl
