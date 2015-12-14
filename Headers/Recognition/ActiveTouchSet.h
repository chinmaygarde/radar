// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_RECOGNITION_ACTIVETOUCHSET_H_
#define RADARLOVE_RECOGNITION_ACTIVETOUCHSET_H_

#include <Core/Core.h>
#include <Event/TouchEvent.h>
#include <Interface/Entity.h>
#include <Layout/Constraint.h>

namespace rl {
namespace recognition {

class ActiveTouchSet {
 public:
  using ProxyConstraintCallback =
      std::function<void(const std::vector<layout::Constraint>&)>;

  ActiveTouchSet(ProxyConstraintCallback addCallback,
                 ProxyConstraintCallback removeCallback);

  void registerProxyConstraint(layout::Constraint&& constraint);

  void applyTouchMap(const event::TouchEvent::PhaseMap& map);

 private:
  using ConstraintConditionsMap = std::map<layout::Constraint,
                                           std::set<layout::Variable::Proxy>,
                                           layout::Constraint::Compare>;
  using ConditionConstraintsMap = std::map<std::set<layout::Variable::Proxy>,
                                           std::vector<layout::Constraint>>;
  using IdentifierEntityMap = std::map<interface::Entity::Identifier,
                                       std::unique_ptr<interface::Entity>>;

  ProxyConstraintCallback _addConstraintCallback;
  ProxyConstraintCallback _removeConstraintCallback;
  IdentifierEntityMap _touchEntities;
  std::vector<event::TouchEvent::Identifier> _indexedTouches;
  ConstraintConditionsMap _conditionsByConstraint;
  ConditionConstraintsMap _activeConstraintsByCondition;

  void addTouches(const std::vector<event::TouchEvent>& touches);
  void updateTouches(const std::vector<event::TouchEvent>& touches);
  void clearTouches(const std::vector<event::TouchEvent>& touches);

  void setupConstraintsForProxies();
  void clearConstraintsForProxies();

  void addConstraintForProxy(const layout::Constraint& proxyConstraint);
  void removeConstraintForProxy(const layout::Constraint& proxyConstraint);

  using ConstraintOperation =
      std::function<void(const layout::Constraint&,
                         const std::set<layout::Variable::Proxy>&)>;
  void performOperationOnProxiesSatisfyingCurrentCondition(
      ConstraintOperation operation);
  layout::Variable resolvedVariableForProxy(const layout::Variable& variable);

  interface::Entity* touchEntityForProxy(layout::Variable::Proxy proxy) const;
  interface::Entity* touchEntityForTouchNumber(size_t index) const;

  size_t size() const;

  RL_DISALLOW_COPY_AND_ASSIGN(ActiveTouchSet);
};

}  // namespace recognition
}  // namespace rl

#endif  // RADARLOVE_RECOGNITION_ACTIVETOUCHSET_H_
