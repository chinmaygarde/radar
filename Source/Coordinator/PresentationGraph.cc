// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Animation/Interpolator.h>
#include <Coordinator/PresentationGraph.h>
#include <Coordinator/TransactionPayload.h>

namespace rl {
namespace coordinator {

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

void PresentationGraph::onActionCommit(interface::Action& action) {
  /*
   *  Nothing to do on its own. Its only when we see transfer records with this
   *  action do we need to do some configuration.
   */
}

void PresentationGraph::onTransferRecordCommit(interface::Action& action,
                                               TransferRecord& record,
                                               const core::ClockPoint& time) {
  if (record.property == interface::Entity::Created) {
    _entities[record.identifier] =
        core::make_unique<PresentationEntity>(record.identifier);
    return;
  }

  if (record.property == interface::Entity::Destroyed) {
    if (_root && _root->identifier() == record.identifier) {
      _root = nullptr;
    }

    _entities.erase(record.identifier);
    return;
  }

  prepareActions(action, *_entities[record.identifier], record, time);
}

void PresentationGraph::onRecognizerCommit(
    recognition::GestureRecognizer::Collection&& recognizers) {
  _recognitionEngine.setupRecognizers(std::move(recognizers));
}

template <typename T>
void PresentationGraph::prepareActionSingle(
    interface::Action& action,
    PresentationEntity& entity,
    const TransferRecord& record,
    const interface::Entity::Accessors<T>& accessors,
    const core::ClockPoint& start) {
  /*
   *  Prepare the key for the animation in the animation director
   */
  animation::Director::Key key(record.identifier, record.property);

  /*
   *  Prepare the interpolator
   */
  auto interpolator = core::make_unique<animation::Interpolator<T>>(
      &entity, action, accessors.setter, accessors.getter(entity),
      record.transferData<T>());

  /*
   *  Setup the intepolator in the animation director
   */
  _animationDirector.setInterpolator(key, std::move(interpolator), start);
}

void PresentationGraph::prepareActions(interface::Action& action,
                                       PresentationEntity& entity,
                                       const TransferRecord& record,
                                       const core::ClockPoint& time) {
  switch (record.property) {
    case interface::Entity::Bounds:
      if (action.propertyMask() & interface::Entity::Bounds) {
        prepareActionSingle(action, entity, record, interface::BoundsAccessors,
                            time);
      } else {
        entity.setBounds(record.data.rect);
      }
      break;
    case interface::Entity::Position:
      if (action.propertyMask() & interface::Entity::Position) {
        prepareActionSingle(action, entity, record,
                            interface::PositionAccessors, time);
      } else {
        entity.setPosition(record.data.point);
      }
      break;
    case interface::Entity::AnchorPoint:
      if (action.propertyMask() & interface::Entity::AnchorPoint) {
        prepareActionSingle(action, entity, record,
                            interface::AnchorPointAccessors, time);
      } else {
        entity.setAnchorPoint(record.data.point);
      }
      break;
    case interface::Entity::Transformation:
      if (action.propertyMask() & interface::Entity::Transformation) {
        prepareActionSingle(action, entity, record,
                            interface::TransformationAccessors, time);
      } else {
        entity.setTransformation(record.data.matrix);
      }
      break;
    case interface::Entity::BackgroundColor:
      if (action.propertyMask() & interface::Entity::BackgroundColor) {
        prepareActionSingle(action, entity, record,
                            interface::BackgroundColorAccessors, time);
      } else {
        entity.setBackgroundColor(record.data.color);
      }
      break;
    case interface::Entity::Opacity:
      if (action.propertyMask() & interface::Entity::Opacity) {
        prepareActionSingle(action, entity, record, interface::OpacityAccessors,
                            time);
      } else {
        entity.setOpacity(record.data.number);
      }
      break;
    case interface::Entity::AddedTo:
      (*_entities[record.data.identifier]).addChild(&entity);
      break;
    case interface::Entity::RemovedFrom:
      (*_entities[record.data.identifier]).removeChild(&entity);
      break;
    case interface::Entity::MakeRoot:
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

animation::Director& PresentationGraph::animationDirector() {
  return _animationDirector;
}

bool PresentationGraph::applyTouchMap(event::TouchEvent::PhaseMap&& map) {
  RL_ASSERT(map.size() > 0);

  return _recognitionEngine.applyTouchMap(std::move(map), _entities);
}

}  // namespace coordinator
}  // namespace rl