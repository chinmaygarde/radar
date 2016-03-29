// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Coordinator/PresentationGraph.h>
#include <Coordinator/TransactionPayload.h>

namespace rl {
namespace coordinator {

PresentationGraph::PresentationGraph(core::Namespace& localNS,
                                     InterfaceStatistics& stats)
    : _localNS(localNS),
      _stats(stats),
      _root(nullptr),
      _layoutSolver(localNS),
      _proxyResolver(_localNS,
                     std::bind(&PresentationGraph::onProxyConstraintsAddition,
                               this,
                               std::placeholders::_1),
                     std::bind(&PresentationGraph::onProxyConstraintsRemoval,
                               this,
                               std::placeholders::_1),
                     std::bind(&PresentationGraph::onSuggestionsCommit,
                               this,
                               std::placeholders::_1),
                     std::bind(&PresentationGraph::resolveConstraintConstant,
                               this,
                               std::placeholders::_1)) {}

PresentationGraph::~PresentationGraph() {}

PresentationEntity& PresentationGraph::presentationEntityForName(
    const core::Name& name) {
  RL_ASSERT(!name.isDead());

  auto& entity = _entities[name];

  /*
   *  TODO: There needs to a consolidation phase for unused entities after
   *        a transaction update.
   */
  if (!entity) {
    entity = core::make_unique<PresentationEntity>(name);
  }

  return *entity;
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
      std::bind(&G::onTransferEntityCommit, this, P::_1, P::_2, P::_3),
      std::bind(&G::onConstraintsCommit, this, P::_1),
      std::bind(&G::onSuggestionsCommit, this, P::_1));
  return payload.deserialize(arena, &_localNS);
}

void PresentationGraph::onActionCommit(animation::Action&) {
  /*
   *  Nothing to do on its own. Its only when we see transfer records with this
   *  action do we need to do some configuration.
   */
  RL_TRACE_AUTO("ActionCommit");
}

void PresentationGraph::onTransferEntityCommit(animation::Action& action,
                                               TransferEntity& transferEntity,
                                               const core::ClockPoint& time) {
  auto& presentationEntity =
      presentationEntityForName(transferEntity.identifier());

  /*
   *  The presentation entity contains the old property values and the
   *  transfer entity contains the new values. If the action update mask
   *  specifies an update for a property, setup an interpolator. Then merge
   *  all enabled properties on the transfer entity.
   */
  using Property = entity::Entity::Property;
  using PropertyMask = entity::Entity::PropertyMask;

  /*

   *  Step 1: Resolve hierarchy updates
   */
  const auto hierarchyMask = PropertyMask::AddedToMask |
                             PropertyMask::RemovedFromMask |
                             PropertyMask::MakeRootMask;

  transferEntity.walkEnabledProperties(hierarchyMask, [&](Property prop) {
    switch (prop) {
      case Property::AddedTo: {
        presentationEntityForName(transferEntity.addedToTarget())
            .addChild(&presentationEntity);
      } break;
      case Property::RemovedFrom: {
        presentationEntityForName(transferEntity.removedFromTarget())
            .removeChild(&presentationEntity);
      } break;
      case Property::MakeRoot:
        _root = &presentationEntity;
        break;
      default:
        RL_ASSERT("Unreachable");
        break;
    }
    return true;
  });

  const auto allowedInterpolatableProperties = action.propertyMask();

  /*
   *  Step 2: Setup interpolators on updated items and properties enabled
   *          via actions mask
   */
  transferEntity.walkEnabledProperties(
      allowedInterpolatableProperties, [&](Property prop) {
        /*
         *  Prepare the key for the animation in the animation director
         */
        animation::Director::Key key(presentationEntity.identifier(), prop);

        /*
         *  Only the animatable properties are cycled over in this callback.
         */
        switch (prop) {
          case Property::Bounds:
            _animationDirector.setInterpolator<geom::Rect>(
                /* key */
                key,
                /* start time */
                time,
                /* action */
                action,
                /* from value */
                presentationEntity.bounds(),
                /* to value */
                transferEntity.bounds(),
                /* stepper */
                std::bind(&PresentationEntity::setBounds, &presentationEntity,
                          std::placeholders::_1));

            break;
          case Property::Position:
            _animationDirector.setInterpolator<geom::Point>(
                /* key */
                key,
                /* start time */
                time,
                /* action */
                action,
                /* from value */
                presentationEntity.position(),
                /* to value */
                transferEntity.position(),
                /* stepper */
                std::bind(&PresentationEntity::setPosition, &presentationEntity,
                          std::placeholders::_1));
            break;
          case Property::AnchorPoint:
            _animationDirector.setInterpolator<geom::Point>(
                /* key */
                key,
                /* start time */
                time,
                /* action */
                action,
                /* from value */
                presentationEntity.anchorPoint(),
                /* to value */
                transferEntity.anchorPoint(),
                /* stepper */
                std::bind(&PresentationEntity::setAnchorPoint,
                          &presentationEntity, std::placeholders::_1));
            break;
          case Property::Transformation:
            _animationDirector.setInterpolator<geom::Matrix>(
                /* key */
                key,
                /* start time */
                time,
                /* action */
                action,
                /* from value */
                presentationEntity.transformation(),
                /* to value */
                transferEntity.transformation(),
                /* stepper */
                std::bind(&PresentationEntity::setTransformation,
                          &presentationEntity, std::placeholders::_1));
            break;
          case Property::BackgroundColor:
            _animationDirector.setInterpolator<entity::Color>(
                /* key */
                key,
                /* start time */
                time,
                /* action */
                action,
                /* from value */
                presentationEntity.backgroundColor(),
                /* to value */
                transferEntity.backgroundColor(),
                /* stepper */
                std::bind(&PresentationEntity::setBackgroundColor,
                          &presentationEntity, std::placeholders::_1));
            break;
          case Property::Opacity:
            _animationDirector.setInterpolator<double>(
                /* key */
                key,
                /* start time */
                time,
                /* action */
                action,
                /* from value */
                presentationEntity.opacity(),
                /* to value */
                transferEntity.opacity(),
                /* stepper */
                std::bind(&PresentationEntity::setOpacity, &presentationEntity,
                          std::placeholders::_1));
            break;
          default:
            RL_ASSERT("Non animatable property encountered.");
            break;
        }
        return true;
      });

  /*
   *  Step 3: Merge non interpolated properties from the transfer entity to the
   *          presentation entity.
   */
  presentationEntity.mergeProperties(transferEntity,
                                     ~allowedInterpolatableProperties);
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

  _stats.constraintsCount().reset(_layoutSolver.constraintsCount());
}

void PresentationGraph::onProxyConstraintsAddition(
    const std::vector<layout::Constraint>& constraints) {
  auto addResult = _layoutSolver.addConstraints(constraints);
  RL_ASSERT(addResult == layout::Result::Success);

  _stats.constraintsCount().reset(_layoutSolver.constraintsCount());
}

void PresentationGraph::onProxyConstraintsRemoval(
    const std::vector<layout::Constraint>& constraints) {
  auto removeResult = _layoutSolver.removeConstraints(constraints);
  RL_ASSERT(removeResult == layout::Result::Success);

  _stats.constraintsCount().reset(_layoutSolver.constraintsCount());
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
        _stats.editVariablesCount().reset(_layoutSolver.editVariableCount());

        suggestionResult = _layoutSolver.applySuggestion(suggestion);
      }
    }

    RL_ASSERT_MSG(suggestionResult == layout::Result::Success,
                  "Must be able to apply constraint suggestion");
  }
}

bool PresentationGraph::render(Frame& frame) {
  if (_root == nullptr) {
    return false;
  }
  frame.statistics().entityCount().increment(_entities.size());
  return _root->render(frame, geom::MatrixIdentity);
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
     *  In case the entity is created as a result of proxy resolution, it will
     *  not be present in the graph owned collection of entities.
     */
    RL_ASSERT_MSG(!variable.isProxy(),
                  "Entity for a non-proxy identifier was not found. There is a "
                  "cleanup error.");
    return;
  }
  /*
   *  Actually update the property.
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
