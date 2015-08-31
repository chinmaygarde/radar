// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/PresentationGraph.h>
#include <Compositor/Interpolator.h>

namespace rl {

PresentationGraph::PresentationGraph() : _entities(), _root(nullptr) {
}

PresentationGraph::~PresentationGraph() {
}

bool PresentationGraph::applyTransactions(Message& arena) {
  auto applyTime = Time::Current();
  do {
    if (!applyTransactionSingle(arena, applyTime)) {
      return false;
    }
  } while (!arena.readCompleted());
  RL_ASSERT(arena.readCompleted());
  return true;
}

bool PresentationGraph::applyTransactionSingle(Message& arena,
                                               std::chrono::nanoseconds time) {
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

    prepareActionsAndMerge(action, *_entities[record.identifier], record, time);
  }

  return true;
}

void PresentationGraph::prepareActionsAndMerge(Action& action,
                                               PresentationEntity& entity,
                                               const TransferRecord& record,
                                               std::chrono::nanoseconds time) {
#define SetInterpolator(prop, getter, propertyType, recordDataType, time) \
  if (action.propertyMask() & prop) {                                     \
    _animationDirector.setInterpolator(                                   \
        {record.identifier, record.property},                             \
        Interpolator<propertyType>(action, entity.getter,                 \
                                   record.data.recordDataType),           \
        time);                                                            \
  }
  switch (record.property) {
    case Entity::Bounds:
      SetInterpolator(Entity::Bounds, bounds(), Rect, rect, time);
      entity.setBounds(record.data.rect);
      break;
    case Entity::Position:
      SetInterpolator(Entity::Position, position(), Point, point, time);
      entity.setPosition(record.data.point);
      break;
    case Entity::AnchorPoint:
      SetInterpolator(Entity::AnchorPoint, anchorPoint(), Point, point, time);
      entity.setAnchorPoint(record.data.point);
      break;
    case Entity::Transformation:
      SetInterpolator(Entity::Transformation, transformation(), Matrix, matrix,
                      time);
      entity.setTransformation(record.data.matrix);
      break;
    case Entity::BackgroundColor:
      SetInterpolator(Entity::BackgroundColor, backgroundColor(), Color, color,
                      time);
      entity.setBackgroundColor(record.data.color);
      break;
    case Entity::Opacity:
      SetInterpolator(Entity::Opacity, opacity(), double, number, time);
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

#undef SetInterpolator
}

void PresentationGraph::render(Frame& frame) {
  if (_root == nullptr) {
    return;
  }
  frame.statistics().entityCount().increment(_entities.size());
  _root->render(frame);
}

}  // namespace rl
