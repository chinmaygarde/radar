// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Layout/ProxyResolver.h>

#include <algorithm>

namespace rl {
namespace layout {

ProxyResolver::ProxyResolver(
    ProxyConstraintCallback addCallback,
    ProxyConstraintCallback removeCallback,
    ProxySuggestionCallback suggestionsCallback,
    Constraint::ConstantResolutionCallback constantResolutionCallback)
    : _addConstraintCallback(addCallback),
      _removeConstraintCallback(removeCallback),
      _suggestionsCallback(suggestionsCallback),
      _constantResolutionCallback(constantResolutionCallback) {
  RL_ASSERT(addCallback != nullptr);
  RL_ASSERT(removeCallback != nullptr);
  RL_ASSERT(suggestionsCallback != nullptr);
  RL_ASSERT(constantResolutionCallback != nullptr);
}

size_t ProxyResolver::size() const {
  return _touchEntities.size();
}

/**
 *  This just wraps the constant resolution callback provided in the constructor
 *  to check if any of the entities inflated as a result of proxy resolution
 *  can take part in constants resolution
 */
double ProxyResolver::constantResolutionCallback(const Variable& variable) {
  /*
   *  Try to resolve the entity in the set of proxy resolved entities.
   */

  for (const auto& entity : _touchEntities) {
    /*
     *  Touch entities are keyed by their proxy identifiers. So we cannot use
     *  std::find
     */
    if (entity.second->identifier() == variable.identifier()) {
      return Variable::GetProperty(*entity.second, variable.property());
    }
  }

  /*
   *  Ask the caller for constant resolution.
   */
  return _constantResolutionCallback(variable);
}

void ProxyResolver::addTouches(const std::vector<event::TouchEvent>& touches) {
  if (touches.size() == 0) {
    return;
  }

  bool addedNewIndexedTouches = false;

  for (const auto& touch : touches) {
    auto identifier = touch.identifier();

    /*
     *  Create a new touch entity for this identifier
     */
    auto touchEntity =
        core::make_unique<interface::Entity>(false /* notifies interface */);
    touchEntity->setBounds({0.0, 0.0, 1.0, 1.0});
    touchEntity->setPosition(touch.location());

    auto identifierTouchPair =
        std::make_pair(identifier, std::move(touchEntity));

    auto res = _touchEntities.insert(std::move(identifierTouchPair));

    if (res.second) {
      addedNewIndexedTouches = true;
      _indexedTouches.push_back(identifier);
    }
  }

  RL_ASSERT(_indexedTouches.size() == _touchEntities.size());

  if (addedNewIndexedTouches) {
    setupConstraintsForProxies();

    /*
     *  Setup initial suggestions
     */
    for (auto& touchEntry : _touchEntities) {
      auto& entity = *touchEntry.second;
      updateEntityPosition(entity, entity.position());
    }
  }
}

void ProxyResolver::clearTouches(
    const std::vector<event::TouchEvent>& touches) {
  if (touches.size() == 0) {
    return;
  }

  clearConstraintsForProxies();

  for (const auto& touch : touches) {
    auto identifier = touch.identifier();

    auto res = _touchEntities.erase(identifier);
    RL_ASSERT_MSG(res != 0, "A touch that was not already active was ended");

    auto found =
        std::find(_indexedTouches.begin(), _indexedTouches.end(), identifier);
    RL_ASSERT(found != _indexedTouches.end());

    _indexedTouches.erase(found);
  }

  RL_ASSERT(_indexedTouches.size() == _touchEntities.size());
}

void ProxyResolver::updateTouches(
    const std::vector<event::TouchEvent>& touches) {
  for (const auto& touch : touches) {
    auto& entity = _touchEntities.at(touch.identifier());
    RL_ASSERT(entity != nullptr);

    updateEntityPosition(*entity, touch.location());
  }
}

void ProxyResolver::updateEntityPosition(interface::Entity& entity,
                                         const geom::Point& position) {
  /*
   *  These are touch entities, so no view matrix conversions are necessary
   */
  entity.setPosition(position);

  Variable positionX = {entity.identifier(), Variable::Property::PositionX};
  Variable positionY = {entity.identifier(), Variable::Property::PositionY};

  auto priority = layout::priority::Required - 1.0;

  _suggestionsCallback(
      {{positionX, position.x, priority}, {positionY, position.y, priority}});
}

interface::Entity* ProxyResolver::touchEntityForProxy(
    Variable::Proxy proxy) const {
  return touchEntityForTouchNumber(static_cast<Variable::ProxyType>(proxy));
}

interface::Entity* ProxyResolver::touchEntityForTouchNumber(
    size_t number) const {
  if (number == 0) {
    return nullptr;
  }

  if (number > _touchEntities.size()) {
    return nullptr;
  }

  const auto& touchEvent = _touchEntities.at(_indexedTouches[number - 1]);
  /*
   *  Paranoid assertion since bounds checking has already been done
   */
  RL_ASSERT(touchEvent != nullptr);

  return touchEvent.get();
}

void ProxyResolver::registerProxyConstraint(Constraint&& constraint) {
  std::set<Variable::Proxy> proxies;

  for (const auto& term : constraint.expression().terms()) {
    auto proxy = term.variable().proxy();
    if (proxy != Variable::Proxy::None) {
      proxies.insert(proxy);
    }
  }

  RL_ASSERT_MSG(proxies.size() > 0,
                "A proxy constraint without any acutal proxy members cannot be "
                "registered");

  auto result = _conditionsByConstraint.emplace(std::move(constraint),
                                                std::move(proxies));
  RL_ASSERT(result.second);
}

static bool ProxyConditionsSatisfied(const std::set<Variable::Proxy>& proxies,
                                     size_t touchCount) {
  return proxies.size() == touchCount;
}

void ProxyResolver::performOperationOnProxiesSatisfyingCurrentCondition(
    ConstraintOperation operation) {
  const auto activeTouches = _indexedTouches.size();
  for (const auto& proxies : _conditionsByConstraint) {
    /*
     *  For each of the proxy constraints, all proxy conditions must be
     *  satisfied
     */
    if (ProxyConditionsSatisfied(proxies.second, activeTouches)) {
      operation(proxies.first, proxies.second);
    }
  }
}

Variable ProxyResolver::resolvedVariableForProxy(const Variable& variable) {
  auto result = touchEntityForProxy(variable.proxy());
  RL_ASSERT(result != nullptr);
  return {result->identifier(), variable.property()};
}

void ProxyResolver::setupConstraintsForProxies() {
  std::vector<Constraint> constraintsToAdd;
  /*
   *  Perform proxy resolution on each constraint for which all conditions are
   *  satisfied
   */
  performOperationOnProxiesSatisfyingCurrentCondition([&](
      const Constraint& proxyConstraint,
      const std::set<Variable::Proxy>& conditions) {
    /*
     *  Resolve the proxy constraint
     */
    Constraint::ProxyVariableReplacementCallback replacement = std::bind(
        &ProxyResolver::resolvedVariableForProxy, this, std::placeholders::_1);
    Constraint::ConstantResolutionCallback constResolution =
        std::bind(&ProxyResolver::constantResolutionCallback, this,
                  std::placeholders::_1);
    auto resolvedConstraint =
        proxyConstraint.resolveProxies(replacement, constResolution);

    /*
     *  Register the resolved constraint for later deletion and notify the
     *  delegate
     */
    _activeConstraintsByCondition[conditions].emplace_back(resolvedConstraint);

    /*
     *  The solver is hosted by the delegate and is responsible for adding
     *  this constraint
     */
    constraintsToAdd.emplace_back(resolvedConstraint);
  });

  if (constraintsToAdd.size() > 0) {
    _addConstraintCallback(std::move(constraintsToAdd));
  }
}

/**
 *  This method is called just before the current condition is about to be
 *  disrupted. All proxies that were inflated for this condition must be cleaned
 *  up.
 */
void ProxyResolver::clearConstraintsForProxies() {
  std::vector<Constraint> constraintsToRemove;
  performOperationOnProxiesSatisfyingCurrentCondition(
      [&](const Constraint& proxyConstraint,
          const std::set<Variable::Proxy>& conditions) {
        /*
         *  Find all the constraints that were previously added for the given
         *  condition and attempt to remove the same from the solver hosted by
         *  the delegate
         */
        auto activeConstraints = _activeConstraintsByCondition[conditions];
        std::move(activeConstraints.begin(), activeConstraints.end(),
                  std::back_inserter(constraintsToRemove));
        _activeConstraintsByCondition.erase(conditions);
      });

  if (constraintsToRemove.size() > 0) {
    _removeConstraintCallback(std::move(constraintsToRemove));
  }
}

void ProxyResolver::applyTouchMap(const event::TouchEvent::PhaseMap& map) {
  using Phase = event::TouchEvent::Phase;

  /*
   *  Phase::Began
   */
  auto found = map.find(Phase::Began);
  if (found != map.end()) {
    RL_ASSERT(found->second.size() != 0);
    addTouches(found->second);
  }

  /*
   *  Phase::Moved
   */
  found = map.find(Phase::Moved);
  if (found != map.end()) {
    RL_ASSERT(found->second.size() != 0);
    updateTouches(found->second);
  }

  /*
   *  Phase::Ended
   */
  found = map.find(Phase::Ended);
  if (found != map.end()) {
    RL_ASSERT(found->second.size() != 0);
    clearTouches(found->second);
  }

  /*
   *  Phase::Cancelled
   */
  found = map.find(Phase::Cancelled);
  if (found != map.end()) {
    RL_ASSERT(found->second.size() != 0);
    clearTouches(found->second);
  }
}

}  // namespace layout
}  // namespace rl
