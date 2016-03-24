// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_COORDINATOR_PRESENTATIONGRAPH_H_
#define RADARLOVE_COORDINATOR_PRESENTATIONGRAPH_H_

#include <Animation/Action.h>
#include <Animation/Director.h>
#include <Coordinator/EntityArena.h>
#include <Coordinator/Frame.h>
#include <Coordinator/PresentationEntity.h>
#include <Coordinator/TransactionPayload.h>
#include <Coordinator/TransferEntity.h>
#include <Core/Core.h>
#include <Layout/ProxyResolver.h>
#include <Layout/Solver.h>
#include <Layout/Suggestion.h>

#include <map>

namespace rl {
namespace coordinator {

class PresentationGraph {
 public:
  explicit PresentationGraph(core::Namespace& localNS);

  ~PresentationGraph();

  bool applyTransactions(core::Message& arena);

  void applyTouchMap(const event::TouchEvent::PhaseMap& touches);

  layout::Solver::FlushResult applyConstraints();

  bool render(Frame& frame);

  animation::Director& animationDirector();

 private:
  core::Namespace& _localNS;
  PresentationEntity::IdentifierMap _entities;
  PresentationEntity* _root;
  animation::Director _animationDirector;
  layout::Solver _layoutSolver;
  layout::ProxyResolver _proxyResolver;

  PresentationEntity& presentationEntityForName(const core::Name& name);

  bool applyTransactionSingle(core::Message& arena,
                              const core::ClockPoint& time);

  template <typename T>
  void prepareActionSingle(const core::ClockPoint& start,
                           interface::Action& action,
                           PresentationEntity& presentationEntity,
                           interface::Entity::Property property,
                           const T& propertyValue,
                           const interface::Entity::Accessors<T>& accessors);

  void onActionCommit(interface::Action& action);

  void onTransferEntityCommit(interface::Action& action,
                              TransferEntity& transferEntity,
                              const core::ClockPoint& commitTime);

  void onConstraintsCommit(std::vector<layout::Constraint>&& constraints);

  void onProxyConstraintsAddition(
      const std::vector<layout::Constraint>& constraint);

  void onProxyConstraintsRemoval(
      const std::vector<layout::Constraint>& constraint);

  void onSuggestionsCommit(std::vector<layout::Suggestion>&& suggestions);

  void resolveConstraintUpdate(const layout::Variable& variable, double value);

  double resolveConstraintConstant(const layout::Variable& variable) const;

  RL_DISALLOW_COPY_AND_ASSIGN(PresentationGraph);
};

}  // namespace coordinator
}  // namespace rl

#endif  // RADARLOVE_COORDINATOR_PRESENTATIONGRAPH_H_