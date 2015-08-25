// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/PresentationGraph.h>

namespace rl {

PresentationGraph::PresentationGraph() {
}

PresentationGraph::~PresentationGraph() {
}

void PresentationGraph::applyUpdates(EntityArena& arena) {
  for (size_t i = 0, end = arena.encodedEntities(); i < end; i++) {
    auto& updated = arena[i];
    /*
     *  First, we try to directly emplace the entity. If we fail, the compositor
     *  already knows of the same. And we need to merge it with the previous
     *  state and actions if necessary.
     */
    auto result = _entities.emplace(updated.identifier(), updated);
    if (!result.second) {
      prepareActionsAndMerge((*(result.first)).second, updated);
    }
  }
}

void PresentationGraph::prepareActionsAndMerge(Entity& currentState,
                                               const Entity& updatedState) {
  currentState.merge(updatedState);
}

void PresentationGraph::render(Frame& frame) {
  frame.statistics().entityCount().increment(_entities.size());
  for (const auto& i : _entities) {
    i.second.render(frame);
  }
}

}  // namespace rl
