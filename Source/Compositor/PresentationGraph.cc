// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/PresentationGraph.h>

namespace rl {

PresentationGraph::PresentationGraph() : _entities() {
}

PresentationGraph::~PresentationGraph() {
}

void PresentationGraph::applyUpdates(Message& arena) {
  RL_ASSERT(arena.size() % sizeof(TransferRecord) == 0);
  const auto transferRecords = arena.size() / sizeof(TransferRecord);
  for (auto i = 0; i < transferRecords; i++) {
    auto allocation = arena.decodeRaw(sizeof(TransferRecord));

    if (allocation == nullptr) {
      break;
    }

    auto& record = *reinterpret_cast<TransferRecord*>(allocation);

    if (record.property == Entity::Created) {
      _entities[record.identifier] =
          rl::make_unique<PresentationEntity>(record.identifier);
      continue;
    }

    if (record.property == Entity::Destroyed) {
      _entities.erase(record.identifier);
      continue;
    }

    prepareActionsAndMerge(*_entities[record.identifier], record);
  }
  RL_ASSERT(arena.readCompleted());
}

void PresentationGraph::prepareActionsAndMerge(PresentationEntity& currentState,
                                               const TransferRecord& record) {
  switch (record.property) {
    case Entity::Bounds:
      currentState.setBounds(record.data.rect);
      break;
    case Entity::Position:
      currentState.setPosition(record.data.point);
      break;
    case Entity::AnchorPoint:
      currentState.setAnchorPoint(record.data.point);
      break;
    case Entity::Transformation:
      currentState.setTransformation(record.data.matrix);
      break;
    case Entity::BackgroundColor:
      currentState.setBackgroundColor(record.data.color);
      break;
    case Entity::Opacity:
      currentState.setOpacity(record.data.number);
      break;
    case Entity::AddedTo:
      (*_entities[record.data.identifier]).addChild(currentState);
      break;
    case Entity::RemovedFrom:
      (*_entities[record.data.identifier]).removeChild(currentState);
      break;
    default:
      RL_ASSERT(false && "Unknown Property");
  }
}

void PresentationGraph::render(Frame& frame) {
  frame.statistics().entityCount().increment(_entities.size());
  for (const auto& i : _entities) {
    i.second->render(frame);
  }
}

}  // namespace rl
