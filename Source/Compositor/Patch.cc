// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Compositor/Patch.h"

namespace rl {

Patch::Patch() : _marks() {
}

void Patch::mark(const Layer& layer, PatchChunk::Command command) {
  auto identifier = reinterpret_cast<PatchChunk::Identifier>(&layer);
  _marks[identifier].insert(command);
}

}  // namespace rl
