// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/PresentationGraph.h>

namespace rl {

PresentationGraph::PresentationGraph() : _layers() {
}

bool PresentationGraph::applyPatches(Messages messages) {
  for (auto& message : messages) {
    bool success = applyPatchSingle(message);
    if (!success) {
      return false;
    }
  }
  return false;
}

bool PresentationGraph::applyPatchSingle(Message& message) {
  assert(message.size() % sizeof(PatchChunk) == 0);
  size_t patchCount = message.size() / sizeof(PatchChunk);

  for (size_t i = 0; i < patchCount; i++) {
    PatchChunk chunk;
    bool result = message.decode(chunk);
    if (!(result && applyChunk(chunk))) {
      return false;
    }
  }

  return true;
}

bool PresentationGraph::applyChunk(const rl::PatchChunk& chunk) {
  assert(false);
  return false;
}

}  // namespace rl
