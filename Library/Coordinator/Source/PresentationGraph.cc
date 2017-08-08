/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Coordinator/PresentationGraph.h>
#include <Coordinator/TransactionPayload.h>
#include <Core/Message.h>

namespace rl {
namespace coordinator {

PresentationGraph::PresentationGraph(core::Namespace& localNS,
                                     const geom::Size& size,
                                     const std::string& debugTag)
    : _localNS(localNS),
      _stats(debugTag),
      _size(size),
      _root(nullptr),
      _layoutSolver(localNS),
      _hasVisualUpdates(false),
      _proxyResolver(_localNS,
                     std::bind(&PresentationGraph::onProxyConstraintsAddition,
                               this,
                               std::placeholders::_1),
                     std::bind(&PresentationGraph::onProxyConstraintsRemoval,
                               this,
                               std::placeholders::_1),
                     std::bind(&PresentationGraph::onEditVariableUpdate,
                               this,
                               std::placeholders::_1,
                               std::placeholders::_2),
                     std::bind(&PresentationGraph::onEditVariableSuggest,
                               this,
                               std::placeholders::_1,
                               std::placeholders::_2),
                     std::bind(&PresentationGraph::resolveConstraintConstant,
                               this,
                               std::placeholders::_1)) {}

PresentationGraph::~PresentationGraph() {}

bool PresentationGraph::updateSize(const geom::Size& size) {
  if (_size == size) {
    return false;
  }

  _hasVisualUpdates = false;

  _size = size;

  if (_root) {
    _root->setFrame({{}, _size});
  }

  updateRootSizeSuggestions();
  return true;
}

void PresentationGraph::updateRootEntity(
    compositor::PresentationEntity* entity) {
  /*
   *  Cleanup the old entity if there is one.
   */
  using Prop = layout::Variable::Property;
  if (_root != nullptr) {
    const auto identifier = _root->identifier();
    _layoutSolver.removeEditVariable({identifier, Prop::BoundsWidth});
    _layoutSolver.removeEditVariable({identifier, Prop::BoundsHeight});
    _layoutSolver.removeEditVariable({identifier, Prop::PositionX});
    _layoutSolver.removeEditVariable({identifier, Prop::PositionY});
  }

  _root = entity;

  if (_root == nullptr) {
    return;
  }

  _root->setFrame({{}, _size});

  /**
   *  Add root edit variables.
   */
  const auto identifier = _root->identifier();
  const auto priority = rl::layout::priority::Strong();

  auto boundsWidth = layout::Variable{identifier, Prop::BoundsWidth};
  auto boundsHeight = layout::Variable{identifier, Prop::BoundsHeight};
  auto positionX = layout::Variable{identifier, Prop::PositionX};
  auto positionY = layout::Variable{identifier, Prop::PositionY};

  _layoutSolver.addEditVariable(boundsWidth, priority);
  _layoutSolver.addEditVariable(boundsHeight, priority);
  _layoutSolver.addEditVariable(positionX, priority);
  _layoutSolver.addEditVariable(positionY, priority);

  updateRootSizeSuggestions();

  syncSolverStats();
}

void PresentationGraph::updateRootSizeSuggestions() {
  if (_root == nullptr) {
    return;
  }

  using Prop = layout::Variable::Property;
  const auto identifier = _root->identifier();

  auto boundsWidth = layout::Variable{identifier, Prop::BoundsWidth};
  auto boundsHeight = layout::Variable{identifier, Prop::BoundsHeight};
  auto positionX = layout::Variable{identifier, Prop::PositionX};
  auto positionY = layout::Variable{identifier, Prop::PositionY};

  auto bounds = _root->bounds();
  auto position = _root->position();

  _layoutSolver.suggestValueForVariable(boundsWidth, bounds.size.width);
  _layoutSolver.suggestValueForVariable(boundsHeight, bounds.size.height);
  _layoutSolver.suggestValueForVariable(positionX, position.x);
  _layoutSolver.suggestValueForVariable(positionY, position.y);
}

compositor::PresentationEntity& PresentationGraph::presentationEntityForName(
    const core::Name& name) {
  RL_ASSERT(!name.isDead());

  auto& entity = _entities[name];

  /*
   *  TODO: There needs to a consolidation phase for unused entities after
   *        a transaction update.
   */
  if (!entity) {
    entity = std::make_unique<compositor::PresentationEntity>(name);
  }

  return *entity;
}

bool PresentationGraph::applyTransactions(core::Message& arena) {
  instrumentation::AutoStopwatchLap lap(_stats.transactionUpdateTimer());
  _stats.transactionPacksCount().increment();

  auto applyTime = core::Clock::now();

  do {
    RL_RETURN_IF_FALSE(applyTransactionSingle(arena, applyTime))
    _stats.transactionsCount().increment();
  } while (!arena.readCompleted());

  _stats.entitiesCount().reset(_entities.size());

  /*
   *  TODO: This must not be based on the value of the deserialization, but
   *  on whether the deserialization actually made any visual difference.
   */
  _hasVisualUpdates = true;
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

  RL_RETURN_IF_FALSE(payload.deserialize(arena, &_localNS));

  return true;
}

void PresentationGraph::onActionCommit(animation::Action&) {
  /*
   *  Nothing to do on its own. Its only when we see transfer records with this
   *  action do we need to do some configuration.
   */
  RL_TRACE_AUTO(__function__);
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
  const auto hierarchyProperties = PropertyMask::AddedToMask |
                                   PropertyMask::RemovedFromMask |
                                   PropertyMask::MakeRootMask;

  transferEntity.walkEnabledProperties(hierarchyProperties, [&](Property prop) {
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
        updateRootEntity(&presentationEntity);
        break;
      default:
        RL_ASSERT_MSG(false, "Unreachable");
        break;
    }
    return true;
  });

  const auto interpolatedProperties = action.propertyMask();

  /*
   *  Step 2: Setup interpolators on updated items and properties enabled
   *          via actions mask
   */
  transferEntity.walkEnabledProperties(
      interpolatedProperties, [&](Property prop) {
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
                std::bind(&compositor::PresentationEntity::setBounds,
                          &presentationEntity, std::placeholders::_1));

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
                std::bind(&compositor::PresentationEntity::setPosition,
                          &presentationEntity, std::placeholders::_1));
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
                std::bind(&compositor::PresentationEntity::setAnchorPoint,
                          &presentationEntity, std::placeholders::_1));
            break;
          case Property::Transformation: {
            /*
             *  Matrices cannot be interpolated directly. Instead, they need
             *  to be decomposed.
             */
            auto fromDecomposition =
                presentationEntity.transformation().decompose();
            auto toDecomposition = transferEntity.transformation().decompose();

            RL_ASSERT(fromDecomposition.first);
            RL_ASSERT(toDecomposition.first);

            _animationDirector.setInterpolator<geom::Matrix::Decomposition>(
                /* key */
                key,
                /* start time */
                time,
                /* action */
                action,
                /* from value */
                fromDecomposition.second,
                /* to value */
                toDecomposition.second,
                /* stepper */
                std::bind(&compositor::PresentationEntity::setTransformation,
                          &presentationEntity, std::placeholders::_1));
          } break;
          case Property::BackgroundColor: {
            /*
             *  Colors cannot be interpolated directly. Instead, they need to
             *  converted into HSB.
             */
            auto from =
                entity::ColorHSB::FromRGB(presentationEntity.backgroundColor());
            auto to =
                entity::ColorHSB::FromRGB(transferEntity.backgroundColor());

            _animationDirector.setInterpolator<entity::ColorHSB>(
                /* key */
                key,
                /* start time */
                time,
                /* action */
                action,
                /* from value */
                from,
                /* to value */
                to,
                /* stepper */
                std::bind(&compositor::PresentationEntity::setBackgroundColor,
                          &presentationEntity, std::placeholders::_1));
          } break;
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
                std::bind(&compositor::PresentationEntity::setOpacity,
                          &presentationEntity, std::placeholders::_1));
            break;
          case Property::StrokeSize:
            _animationDirector.setInterpolator<
                double>(/* key */
                        key,
                        /* start time */
                        time,
                        /* action */
                        action,
                        /* from value */
                        presentationEntity.strokeSize(),
                        /* to value */
                        transferEntity.strokeSize(),
                        /* stepper */
                        std::bind(
                            &compositor::PresentationEntity::setStrokeSize,
                            &presentationEntity, std::placeholders::_1));
            break;
          default:
            RL_ASSERT_MSG(false, "Non animatable property encountered.");
            break;
        }
        return true;
      });

  /*
   *  Step 3: Merge non interpolated properties from the transfer entity to the
   *          presentation entity.
   */
  const auto nonInterpolatedProperties = ~interpolatedProperties;
  presentationEntity.mergeProperties(
      transferEntity,
      transferEntity.enabledProperties() & nonInterpolatedProperties);
}

void PresentationGraph::onConstraintsCommit(
    std::vector<layout::Constraint>&& constraints) {
  RL_TRACE_AUTO(__function__);

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

  syncSolverStats();
}

void PresentationGraph::onProxyConstraintsAddition(
    const std::vector<layout::Constraint>& constraints) {
  auto addResult = _layoutSolver.addConstraints(constraints);
  RL_ASSERT(addResult == layout::Result::Success);
  syncSolverStats();
}

void PresentationGraph::onProxyConstraintsRemoval(
    const std::vector<layout::Constraint>& constraints) {
  auto removeResult = _layoutSolver.removeConstraints(constraints);
  RL_ASSERT(removeResult == layout::Result::Success);
  syncSolverStats();
}

void PresentationGraph::onSuggestionsCommit(
    std::vector<layout::Suggestion>&& suggestions) {
  RL_TRACE_AUTO(__function__);

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

  syncSolverStats();
}

void PresentationGraph::onEditVariableUpdate(const layout::Variable& variable,
                                             bool addOrRemove) {
  auto result = layout::Result::InternalSolverError;

  if (addOrRemove) {
    result =
        _layoutSolver.addEditVariable(variable, layout::priority::Strong());
  } else {
    result = _layoutSolver.removeEditVariable(variable);
  }

  RL_ASSERT(result == layout::Result::Success);

  syncSolverStats();
}

void PresentationGraph::onEditVariableSuggest(const layout::Variable& variable,
                                              double value) {
  auto result = _layoutSolver.suggestValueForVariable(variable, value);
  RL_ASSERT(result == layout::Result::Success);
  syncSolverStats();
}

void PresentationGraph::syncSolverStats() {
  _stats.constraintsCount().reset(_layoutSolver.constraintsCount());
  _stats.editVariablesCount().reset(_layoutSolver.editVariableCount());
}

void PresentationGraph::render(compositor::FrontEndPass& frontEndPass) {
  if (_root == nullptr) {
    return;
  }

  _root->render(frontEndPass, geom::Matrix{});
}

void PresentationGraph::presentStatistics() const {
  _stats.present();
}

bool PresentationGraph::stepInterpolations() {
  const auto count =
      _animationDirector.stepInterpolations(_stats.interpolations());
  _stats.interpolationsCount().reset(count);
  return count > 0;
}

bool PresentationGraph::resolveVisualUpdates() {
  auto value = _hasVisualUpdates;
  _hasVisualUpdates = false;
  return value;
}

bool PresentationGraph::applyTouchMap(
    const event::TouchEvent::PhaseMap& touches) {
  return _proxyResolver.applyTouchMap(touches);
}

size_t PresentationGraph::applyConstraints() {
  namespace P = std::placeholders;
  return _layoutSolver.flushUpdates(std::bind(
      &PresentationGraph::resolveConstraintUpdate, this, P::_1, P::_2));
}

layout::Solver::FlushResult PresentationGraph::resolveConstraintUpdate(
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
    return layout::Solver::FlushResult::NoUpdates;
  }
  /*
   *  Actually update the property.
   */
  layout::Variable::SetProperty(*found->second, variable.property(), value);
  return layout::Solver::FlushResult::Updated;
}

double PresentationGraph::resolveConstraintConstant(
    const layout::Variable& variable) const {
  auto found = _entities.find(variable.identifier());
  RL_ASSERT(found != _entities.end());
  return layout::Variable::GetProperty(*found->second, variable.property());
}

}  // namespace coordinator
}  // namespace rl
