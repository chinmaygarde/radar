// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/PresentationGraph.h>

namespace rl {

PresentationGraph::PresentationGraph() : _entities() {
}

PresentationGraph::~PresentationGraph() {
}

void PresentationGraph::applyUpdates(EntityArena& arena) {
  for (size_t i = 0, end = arena.encodedEntities(); i < end; i++) {
    const auto& updated = arena[i];
    /*
     *  First, we try to directly emplace the entity. If we fail, the compositor
     *  already knows of the same and we need to merge it with the previous
     *  state and add actions if necessary.
     */
    const auto& found = _entities.find(updated.identifier());
    if (found == _entities.end()) {
      _entities.emplace(updated.identifier(),
                        rl::make_unique<PresentationEntity>(updated));
    } else {
      prepareActionsAndMerge(*((*found).second), updated);
    }
  }
}

void PresentationGraph::prepareActionsAndMerge(
    PresentationEntity& currentState,
    const TransferEntity& updatedState) {
  currentState.merge(updatedState);
}

void PresentationGraph::render(Frame& frame) {
  frame.statistics().entityCount().increment(_entities.size());
  for (const auto& i : _entities) {
    i.second->render(frame);
  }
}

}  // namespace rl
