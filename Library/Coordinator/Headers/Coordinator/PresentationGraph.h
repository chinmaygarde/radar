// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_COORDINATOR_PRESENTATIONGRAPH_H_
#define RADARLOVE_COORDINATOR_PRESENTATIONGRAPH_H_

#include <Animation/Action.h>
#include <Animation/Interpolator.h>
#include <Animation/Director.h>
#include <Coordinator/EntityArena.h>
#include <Coordinator/Frame.h>
#include <Coordinator/InterfaceStatistics.h>
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
  explicit PresentationGraph(core::Namespace& localNS,
                             const geom::Size& size,
                             InterfaceStatistics& stats);

  ~PresentationGraph();

  void updateSize(const geom::Size& size);

  bool applyTransactions(core::Message& arena);

  void applyTouchMap(const event::TouchEvent::PhaseMap& touches);

  layout::Solver::FlushResult applyConstraints();

  bool render(Frame& frame);

  animation::Director& animationDirector();

 private:
  using IdentifierPresentationEntityMap =
      std::map<core::Name, std::unique_ptr<PresentationEntity>>;

  core::Namespace& _localNS;
  InterfaceStatistics& _stats;
  IdentifierPresentationEntityMap _entities;
  geom::Size _size;
  PresentationEntity* _root;
  animation::Director _animationDirector;
  layout::Solver _layoutSolver;
  layout::ProxyResolver _proxyResolver;

  void updateRootEntity(PresentationEntity* entity);

  void updateRootSizeSuggestions();

  PresentationEntity& presentationEntityForName(const core::Name& name);

  bool applyTransactionSingle(core::Message& arena,
                              const core::ClockPoint& time);

  void onActionCommit(animation::Action& action);

  void onTransferEntityCommit(animation::Action& action,
                              TransferEntity& transferEntity,
                              const core::ClockPoint& commitTime);

  void onConstraintsCommit(std::vector<layout::Constraint>&& constraints);

  void onProxyConstraintsAddition(
      const std::vector<layout::Constraint>& constraint);

  void onProxyConstraintsRemoval(
      const std::vector<layout::Constraint>& constraint);

  void onSuggestionsCommit(std::vector<layout::Suggestion>&& suggestions);

  void onEditVariableUpdate(const layout::Variable& variable, bool addOrRemove);

  void onEditVariableSuggest(const layout::Variable& variable, double value);

  void resolveConstraintUpdate(const layout::Variable& variable, double value);

  double resolveConstraintConstant(const layout::Variable& variable) const;

  void syncSolverStats();

  RL_DISALLOW_COPY_AND_ASSIGN(PresentationGraph);
};

}  // namespace coordinator
}  // namespace rl

#endif  // RADARLOVE_COORDINATOR_PRESENTATIONGRAPH_H_
