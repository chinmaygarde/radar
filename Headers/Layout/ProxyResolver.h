// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_RECOGNITION_PROXYRESOLVER_H_
#define RADARLOVE_RECOGNITION_PROXYRESOLVER_H_

#include <Core/Core.h>
#include <Event/TouchEvent.h>
#include <Interface/Entity.h>
#include <Layout/Constraint.h>
#include <Layout/Suggestion.h>

namespace rl {
namespace layout {

class ProxyResolver {
 public:
  using ProxyConstraintCallback =
      std::function<void(const std::vector<Constraint>&)>;
  using ProxySuggestionCallback =
      std::function<void(std::vector<Suggestion>&&)>;

  ProxyResolver(
      ProxyConstraintCallback addCallback,
      ProxyConstraintCallback removeCallback,
      ProxySuggestionCallback suggestionsCallback,
      Constraint::ConstantResolutionCallback constantResolutionCallback);

  void registerProxyConstraint(Constraint&& constraint);

  void applyTouchMap(const event::TouchEvent::PhaseMap& map);

 private:
  using ConstraintConditionsMap =
      std::map<Constraint, std::set<Variable::Proxy>, Constraint::Compare>;
  using ConditionConstraintsMap =
      std::map<std::set<Variable::Proxy>, std::vector<Constraint>>;
  using IdentifierEntityMap = std::map<event::TouchEvent::Identifier,
                                       std::unique_ptr<interface::Entity>>;
  using ConstraintOperation =
      std::function<void(const Constraint&, const std::set<Variable::Proxy>&)>;

  interface::Identifier::Factory _identifierFactory;
  ProxyConstraintCallback _addConstraintCallback;
  ProxyConstraintCallback _removeConstraintCallback;
  ProxySuggestionCallback _suggestionsCallback;
  Constraint::ConstantResolutionCallback _constantResolutionCallback;
  IdentifierEntityMap _touchEntities;
  std::vector<event::TouchEvent::Identifier> _indexedTouches;
  ConstraintConditionsMap _conditionsByConstraint;
  ConditionConstraintsMap _activeConstraintsByCondition;

  void addTouches(const std::vector<event::TouchEvent>& touches);
  void updateTouches(const std::vector<event::TouchEvent>& touches);
  void clearTouches(const std::vector<event::TouchEvent>& touches);

  void setupConstraintsForProxies();
  void clearConstraintsForProxies();

  void updateEntityPosition(interface::Entity& entity,
                            const geom::Point& position);
  void performOperationOnProxiesSatisfyingCurrentCondition(
      ConstraintOperation operation);

  Variable resolvedVariableForProxy(const Variable& variable);
  double constantResolutionCallback(const Variable& variable);

  interface::Entity* touchEntityForProxy(Variable::Proxy proxy) const;
  interface::Entity* touchEntityForTouchNumber(size_t index) const;

  size_t size() const;

  RL_DISALLOW_COPY_AND_ASSIGN(ProxyResolver);
};

}  // namespace layout
}  // namespace rl

#endif  // RADARLOVE_RECOGNITION_PROXYRESOLVER_H_
