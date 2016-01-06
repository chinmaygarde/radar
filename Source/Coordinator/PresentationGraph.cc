// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Animation/Interpolator.h>
#include <Coordinator/PresentationGraph.h>
#include <Coordinator/TransactionPayload.h>

namespace rl {
namespace coordinator {

PresentationGraph::PresentationGraph()
    : _root(nullptr),
      _proxyResolver(
          // clang-format off
          std::bind(&PresentationGraph::onProxyConstraintsAddition,
                    this, std::placeholders::_1),
          std::bind(&PresentationGraph::onProxyConstraintsRemoval,
                    this, std::placeholders::_1),
          std::bind(&PresentationGraph::onSuggestionsCommit,
                    this, std::placeholders::_1),
          std::bind(&PresentationGraph::resolveConstraintConstant,
                    this, std::placeholders::_1)
          // clang-format on
          ) {}

PresentationGraph::~PresentationGraph() {}

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
      std::bind(&G::onConstraintsCommit, this, P::_1),
      std::bind(&G::onSuggestionsCommit, this, P::_1));
  return payload.deserialize(arena);
}

void PresentationGraph::onActionCommit(interface::Action& action) {
  /*
   *  Nothing to do on its own. Its only when we see transfer records with this
   *  action do we need to do some configuration.
   */
  RL_TRACE_AUTO("ActionCommit");
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

  prepareActions(action, *_entities.at(record.identifier), record, time);
}

void PresentationGraph::onConstraintsCommit(
    std::vector<layout::Constraint>&& constraints) {
  RL_TRACE_AUTO("ConstraintsCommit");
  for (auto& constraint : constraints) {
    if (constraint.hasProxies()) {
      /*
       *  If the constraint has proxies, they are collected for and applied
       *  later as the proxies are resolved
       */
      _proxyResolver.registerProxyConstraint(std::move(constraint));
    } else {
      /*
       *  If there are no proxy variable, use direct constraint application
       */
      auto addResult = _layoutSolver.addConstraint(std::move(constraint));
      RL_ASSERT(addResult == layout::Result::Success);
    }
  }
}

void PresentationGraph::onProxyConstraintsAddition(
    const std::vector<layout::Constraint>& constraints) {
  auto addResult = _layoutSolver.addConstraints(constraints);
  RL_ASSERT(addResult == layout::Result::Success);
}

void PresentationGraph::onProxyConstraintsRemoval(
    const std::vector<layout::Constraint>& constraints) {
  auto removeResult = _layoutSolver.removeConstraints(constraints);
  RL_ASSERT(removeResult == layout::Result::Success);
}

void PresentationGraph::onSuggestionsCommit(
    std::vector<layout::Suggestion>&& suggestions) {
  RL_TRACE_AUTO("ConstraintSuggestionsCommit");
  for (const auto& suggestion : suggestions) {
    /*
     *  If the edit variable for the suggestion does not exist yet in the
     *  solver. Add it and make a suggestion.
     */
    auto suggestionResult = _layoutSolver.applySuggestion(suggestion);

    if (suggestionResult == layout::Result::UnknownEditVariable) {
      const auto editAddResult = _layoutSolver.addEditVariable(
          suggestion.variable(), suggestion.priority());

      if (editAddResult == layout::Result::Success) {
        suggestionResult = _layoutSolver.applySuggestion(suggestion);
      }
    }

    RL_ASSERT_MSG(suggestionResult == layout::Result::Success,
                  "Must be able to apply constraint suggestion");
  }
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

void PresentationGraph::applyTouchMap(
    const event::TouchEvent::PhaseMap& touches) {
  _proxyResolver.applyTouchMap(touches);
}

layout::Solver::FlushResult PresentationGraph::applyConstraints() {
  namespace P = std::placeholders;
  return _layoutSolver.flushUpdates(std::bind(
      &PresentationGraph::resolveConstraintUpdate, this, P::_1, P::_2));
}

void PresentationGraph::resolveConstraintUpdate(
    const layout::Variable& variable,
    double value) {
  auto found = _entities.find(variable.identifier());
  if (found == _entities.end()) {
    /*
     *  In case the entity created as a result of proxy resolution, it will not
     *  be present in the graph owned collection of entities.
     */
    return;
  }
  /*
   *  Actually update the property
   */
  layout::Variable::SetProperty(*found->second, variable.property(), value);
}

double PresentationGraph::resolveConstraintConstant(
    const layout::Variable& variable) const {
  auto found = _entities.find(variable.identifier());
  RL_ASSERT(found != _entities.end());
  return layout::Variable::GetProperty(*found->second, variable.property());
}

}  // namespace coordinator
}  // namespace rl
