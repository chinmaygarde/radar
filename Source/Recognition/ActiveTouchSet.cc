// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Recognition/ActiveTouchSet.h>

#include <algorithm>

namespace rl {
namespace recognition {

ActiveTouchSet::ActiveTouchSet(ProxyConstraintCallback addCallback,
                               ProxyConstraintCallback removeCallback)
    : _addConstraintCallback(addCallback),
      _removeConstraintCallback(removeCallback) {
  RL_ASSERT(addCallback != nullptr);
  RL_ASSERT(removeCallback != nullptr);
}

size_t ActiveTouchSet::size() const {
  return _touchEntities.size();
}

void ActiveTouchSet::addTouches(const std::vector<event::TouchEvent>& touches) {
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
  }
}

void ActiveTouchSet::clearTouches(
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

void ActiveTouchSet::updateTouches(
    const std::vector<event::TouchEvent>& touches) {
  for (const auto& touch : touches) {
    auto& entity = _touchEntities.at(touch.identifier());
    RL_ASSERT(entity != nullptr);
    /*
     *  These are touch entities, so no view matrix conversions are necessary
     */
    entity->setPosition(touch.location());
  }
}

interface::Entity* ActiveTouchSet::touchEntityForProxy(
    layout::Variable::Proxy proxy) const {
  return touchEntityForTouchNumber(
      static_cast<layout::Variable::ProxyType>(proxy));
}

interface::Entity* ActiveTouchSet::touchEntityForTouchNumber(
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

void ActiveTouchSet::registerProxyConstraint(layout::Constraint&& constraint) {
  std::set<layout::Variable::Proxy> proxies;

  for (const auto& term : constraint.expression().terms()) {
    auto proxy = term.variable().proxy();
    if (proxy != layout::Variable::Proxy::None) {
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

static bool ProxyConditionsSatisfied(
    const std::set<layout::Variable::Proxy>& proxies,
    size_t touchCount) {
  return proxies.size() == touchCount;
}

void ActiveTouchSet::performOperationOnProxiesSatisfyingCurrentCondition(
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

layout::Variable ActiveTouchSet::resolvedVariableForProxy(
    const layout::Variable& variable) {
  auto result = touchEntityForProxy(variable.proxy());
  RL_ASSERT(result != nullptr);
  return {result->identifier(), variable.property()};
}

void ActiveTouchSet::setupConstraintsForProxies() {
  std::vector<layout::Constraint> constraintsToAdd;
  /*
   *  Perform proxy resolution on each constraint for which all conditions are
   *  satisfied
   */
  performOperationOnProxiesSatisfyingCurrentCondition([&](
      const layout::Constraint& proxyConstraint,
      const std::set<layout::Variable::Proxy>& conditions) {
    /*
     *  Resolve the proxy constraint
     */
    layout::Constraint::ProxyVariableReplacementCallback replacement =
        std::bind(&ActiveTouchSet::resolvedVariableForProxy, this,
                  std::placeholders::_1);
    auto resolvedConstraint = proxyConstraint.resolveProxies(replacement);

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
void ActiveTouchSet::clearConstraintsForProxies() {
  std::vector<layout::Constraint> constraintsToRemove;
  performOperationOnProxiesSatisfyingCurrentCondition(
      [&](const layout::Constraint& proxyConstraint,
          const std::set<layout::Variable::Proxy>& conditions) {
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

void ActiveTouchSet::addConstraintForProxy(
    const layout::Constraint& proxyConstraint) {}

void ActiveTouchSet::removeConstraintForProxy(
    const layout::Constraint& proxyConstraint) {}

void ActiveTouchSet::applyTouchMap(const event::TouchEvent::PhaseMap& map) {
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

}  // namespace recognition
}  // namespace rl
