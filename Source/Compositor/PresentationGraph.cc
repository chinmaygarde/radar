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

bool PresentationGraph::applyTransactionSingle(
    Message& arena,
    const std::chrono::nanoseconds& time) {
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
    auto& record = TransferRecord::NextInMessage(arena);

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

    prepareActions(action, *_entities[record.identifier], record, time);
  }

  return true;
}

template <typename T>
void PresentationGraph::prepareActionSingle(
    Action& action,
    PresentationEntity& entity,
    const TransferRecord& record,
    const Entity::Accessors<T>& accessors,
    const std::chrono::nanoseconds& start) {
  /*
   *  Prepare the key for the animation in the animation director
   */
  AnimationDirector::Key key(record.identifier, record.property);

  /*
   *  Prepare the interpolator
   */
  auto interpolator = rl::make_unique<Interpolator<T>>(
      &entity, action, accessors.setter, accessors.getter(entity),
      record.transferData<T>());

  /*
   *  Setup the intepolator in the animation director
   */
  _animationDirector.setInterpolator(key, std::move(interpolator), start);
}

void PresentationGraph::prepareActions(Action& action,
                                       PresentationEntity& entity,
                                       const TransferRecord& record,
                                       const std::chrono::nanoseconds& time) {
  switch (record.property) {
    case Entity::Bounds:
      if (action.propertyMask() & Entity::Bounds) {
        prepareActionSingle(action, entity, record, BoundsAccessors, time);
      } else {
        entity.setBounds(record.data.rect);
      }
      break;
    case Entity::Position:
      if (action.propertyMask() & Entity::Position) {
        prepareActionSingle(action, entity, record, PositionAccessors, time);
      } else {
        entity.setPosition(record.data.point);
      }
      break;
    case Entity::AnchorPoint:
      if (action.propertyMask() & Entity::AnchorPoint) {
        prepareActionSingle(action, entity, record, AnchorPointAccessors, time);
      } else {
        entity.setAnchorPoint(record.data.point);
      }
      break;
    case Entity::Transformation:
      if (action.propertyMask() & Entity::Transformation) {
        prepareActionSingle(action, entity, record, TransformationAccessors,
                            time);
      } else {
        entity.setTransformation(record.data.matrix);
      }
      break;
    case Entity::BackgroundColor:
      if (action.propertyMask() & Entity::BackgroundColor) {
        prepareActionSingle(action, entity, record, BackgroundColorAccessors,
                            time);
      } else {
        entity.setBackgroundColor(record.data.color);
      }
      break;
    case Entity::Opacity:
      if (action.propertyMask() & Entity::Opacity) {
        prepareActionSingle(action, entity, record, OpacityAccessors, time);
      } else {
        entity.setOpacity(record.data.number);
      }
      break;
    case Entity::AddedTo:
      (*_entities[record.data.identifier]).addChild(&entity);
      break;
    case Entity::RemovedFrom:
      (*_entities[record.data.identifier]).removeChild(&entity);
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

AnimationDirector& PresentationGraph::animationDirector() {
  return _animationDirector;
}

}  // namespace rl
