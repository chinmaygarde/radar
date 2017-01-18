// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_RECOGNITION_PROXYRESOLVER_H_
#define RADAR_RECOGNITION_PROXYRESOLVER_H_

#include <Core/Core.h>
#include <Entity/Entity.h>
#include <Event/TouchEvent.h>
#include <Layout/Constraint.h>
#include <Layout/Suggestion.h>

namespace rl {
namespace layout {

class ProxyResolver {
 public:
  using ProxyConstraintCallback =
      std::function<void(const std::vector<Constraint>&)>;
  using ProxyEditUpdateCallback =
      std::function<void(const Variable& /* variable */,
                         bool /* addOrRemove */)>;
  using ProxyEditSuggestCallback =
      std::function<void(const Variable& /* variable */, double /* value */)>;

  ProxyResolver(
      core::Namespace& localNS,
      ProxyConstraintCallback addCallback,
      ProxyConstraintCallback removeCallback,
      ProxyEditUpdateCallback editUpdateCallback,
      ProxyEditSuggestCallback editSuggetCallback,
      Constraint::ConstantResolutionCallback constantResolutionCallback);

  void registerProxyConstraint(Constraint&& constraint);

  bool applyTouchMap(const event::TouchEvent::PhaseMap& map);

 private:
  using ConstraintConditionsMap =
      std::map<Constraint, std::set<Variable::Proxy>, Constraint::Compare>;
  using ConditionConstraintsMap =
      std::map<std::set<Variable::Proxy>, std::vector<Constraint>>;
  using IdentifierEntityMap =
      std::map<event::TouchEvent::Identifier, std::unique_ptr<entity::Entity>>;
  using ConstraintOperation =
      std::function<void(const Constraint&, const std::set<Variable::Proxy>&)>;

  core::Namespace& _localNS;
  ProxyConstraintCallback _addConstraintCallback;
  ProxyConstraintCallback _removeConstraintCallback;
  ProxyEditUpdateCallback _editUpdateCallback;
  ProxyEditSuggestCallback _editSuggestCallback;
  Constraint::ConstantResolutionCallback _constantResolutionCallback;
  IdentifierEntityMap _touchEntities;
  std::vector<event::TouchEvent::Identifier> _indexedTouches;
  ConstraintConditionsMap _conditionsByConstraint;
  ConditionConstraintsMap _activeConstraintsByCondition;

  bool addTouches(const std::vector<event::TouchEvent>& touches);
  bool updateTouches(const std::vector<event::TouchEvent>& touches);
  bool clearTouches(const std::vector<event::TouchEvent>& touches);

  void setupConstraintsForProxies();
  void clearConstraintsForProxies();

  void updateEntityPosition(entity::Entity& entity,
                            const geom::Point& position);
  void performOperationOnProxiesSatisfyingCurrentCondition(
      ConstraintOperation operation);

  Variable resolvedVariableForProxy(const Variable& variable);

  double constantResolutionCallback(const Variable& variable);

  void reportTouchEditsToDelegate(const core::Name& identifier,
                                  bool addOrRemove);

  entity::Entity* touchEntityForProxy(Variable::Proxy proxy) const;
  entity::Entity* touchEntityForTouchNumber(size_t index) const;

  size_t size() const;

  RL_DISALLOW_COPY_AND_ASSIGN(ProxyResolver);
};

}  // namespace layout
}  // namespace rl

#endif  // RADAR_RECOGNITION_PROXYRESOLVER_H_
