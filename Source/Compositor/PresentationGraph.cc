// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/PresentationGraph.h>
#include <Interface/Action.h>

namespace rl {

PresentationGraph::PresentationGraph() : _entities(), _root(nullptr) {
}

PresentationGraph::~PresentationGraph() {
}

bool PresentationGraph::applyTransactions(Message& arena) {
  do {
    if (!applyTransactionSingle(arena)) {
      return false;
    }
  } while (!arena.readCompleted());
  RL_ASSERT(arena.readCompleted());
  return true;
}

bool PresentationGraph::applyTransactionSingle(Message& arena) {
  bool result = true;
  /*
   *  Step 1: Read the action
   */
  Action action;
  result = action.deserialize(arena);

  if (!result) {
    return false;
  }

  /*
   *  Step 2: Read the transfer record count
   */
  size_t transferRecords = 0;
  result = arena.decode(transferRecords);
  if (!result) {
    return false;
  }

  /*
   *  Step 3: Read the transfer records
   */
  for (auto i = 0; i < transferRecords; i++) {
    auto allocation = arena.decodeRawUnsafe(sizeof(TransferRecord));

    if (allocation == nullptr) {
      return false;
    }

    auto& record = *reinterpret_cast<TransferRecord*>(allocation);

    if (record.property == Entity::Created) {
      _entities[record.identifier] =
          rl::make_unique<PresentationEntity>(record.identifier);
      continue;
    }

    if (record.property == Entity::Destroyed) {
      if (_root && _root->identifier() == record.identifier) {
        _root = nullptr;
      }

      _entities.erase(record.identifier);
      continue;
    }

    prepareActionsAndMerge(*_entities[record.identifier], record);
  }

  return true;
}

void PresentationGraph::prepareActionsAndMerge(PresentationEntity& entity,
                                               const TransferRecord& record) {
  switch (record.property) {
    case Entity::Bounds:
      entity.setBounds(record.data.rect);
      break;
    case Entity::Position:
      entity.setPosition(record.data.point);
      break;
    case Entity::AnchorPoint:
      entity.setAnchorPoint(record.data.point);
      break;
    case Entity::Transformation:
      entity.setTransformation(record.data.matrix);
      break;
    case Entity::BackgroundColor:
      entity.setBackgroundColor(record.data.color);
      break;
    case Entity::Opacity:
      entity.setOpacity(record.data.number);
      break;
    case Entity::AddedTo:
      (*_entities[record.data.identifier]).addChild(entity);
      break;
    case Entity::RemovedFrom:
      (*_entities[record.data.identifier]).removeChild(entity);
      break;
    case Entity::MakeRoot:
      _root = &entity;
      break;
    default:
      RL_ASSERT(false && "Unknown Property");
  }
}

void PresentationGraph::render(Frame& frame) {
  if (_root == nullptr) {
    return;
  }
  frame.statistics().entityCount().increment(_entities.size());
  _root->render(frame);
}

}  // namespace rl
