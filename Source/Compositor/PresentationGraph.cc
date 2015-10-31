// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/PresentationGraph.h>
#include <Compositor/Interpolator.h>
#include <Compositor/TransactionPayload.h>

namespace rl {

PresentationGraph::PresentationGraph() : _root(nullptr) {
}

PresentationGraph::~PresentationGraph() {
}

bool PresentationGraph::applyTransactions(core::Message& arena) {
  auto applyTime = core::Clock::now();
  do {
    if (!applyTransactionSingle(arena, applyTime)) {
      return false;
    }
  } while (!arena.readCompleted());
  RL_ASSERT(arena.readCompleted());
  return true;
}

bool PresentationGraph::applyTransactionSingle(core::Message& arena,
                                               const core::ClockPoint& time) {
  namespace P = std::placeholders;
  using G = PresentationGraph;
  TransactionPayload payload(
      time,  //
      std::bind(&G::onActionCommit, this, P::_1),
      std::bind(&G::onTransferRecordCommit, this, P::_1, P::_2, P::_3),
      std::bind(&G::onRecognizerCommit, this, P::_1));
  return payload.deserialize(arena);
}

void PresentationGraph::onActionCommit(Action& action) {
  /*
   *  Nothing to do on its own. Its only when we see transfer records with this
   *  action do we need to do some configuration.
   */
}

void PresentationGraph::onTransferRecordCommit(Action& action,
                                               TransferRecord& record,
                                               const core::ClockPoint& time) {
  if (record.property == Entity::Created) {
    _entities[record.identifier] =
        core::make_unique<PresentationEntity>(record.identifier);
    return;
  }

  if (record.property == Entity::Destroyed) {
    if (_root && _root->identifier() == record.identifier) {
      _root = nullptr;
    }

    _entities.erase(record.identifier);
    return;
  }

  prepareActions(action, *_entities[record.identifier], record, time);
}

void PresentationGraph::onRecognizerCommit(
    GestureRecognizer::Collection&& recognizers) {
  _recognitionEngine.setupRecognizers(std::move(recognizers));
}

template <typename T>
void PresentationGraph::prepareActionSingle(
    Action& action,
    PresentationEntity& entity,
    const TransferRecord& record,
    const Entity::Accessors<T>& accessors,
    const core::ClockPoint& start) {
  /*
   *  Prepare the key for the animation in the animation director
   */
  AnimationDirector::Key key(record.identifier, record.property);

  /*
   *  Prepare the interpolator
   */
  auto interpolator = core::make_unique<Interpolator<T>>(
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
                                       const core::ClockPoint& time) {
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
      RL_ASSERT_MSG(false, "Unknown Property");
  }
}

void PresentationGraph::render(Frame& frame) {
  if (_root == nullptr) {
    return;
  }
  frame.statistics().entityCount().increment(_entities.size());
  _root->render(frame, geom::MatrixIdentity);
}

AnimationDirector& PresentationGraph::animationDirector() {
  return _animationDirector;
}

bool PresentationGraph::applyTouchMap(event::TouchEvent::PhaseMap&& map) {
  RL_ASSERT(map.size() > 0);

  return _recognitionEngine.applyTouchMap(std::move(map), _entities);
}

}  // namespace rl
