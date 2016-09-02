// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_COORDINATOR_PRESENTATIONGRAPH_H_
#define RADARLOVE_COORDINATOR_PRESENTATIONGRAPH_H_

#include <Animation/Action.h>
#include <Animation/Interpolator.h>
#include <Animation/Director.h>
#include <Compositor/FrontendPass.h>
#include <Compositor/InterfaceStatistics.h>
#include <Compositor/PresentationEntity.h>
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
  PresentationGraph(core::Namespace& localNS,
                    const geom::Size& size,
                    const std::string& debugTag);

  ~PresentationGraph();

  bool updateSize(const geom::Size& size);

  bool applyTransactions(core::Message& arena);

  bool applyTouchMap(const event::TouchEvent::PhaseMap& touches);

  /**
   *  Applies pending constraint updates to the presentation graph and returns
   *  the number of visual entities on which constraint updates were performed.
   *
   *  @return the number of visual entities on which constraint updates were
   *          applied. Most notably, touch entity updates do NOT count to this
   *          number.
   */
  size_t applyConstraints();

  bool stepInterpolations();

  bool resolveVisualUpdates();

  void render(compositor::FrontEndPass& frontEndPass);

 private:
  using IdentifierPresentationEntityMap =
      std::map<core::Name, std::unique_ptr<compositor::PresentationEntity>>;

  core::Namespace& _localNS;
  compositor::InterfaceStatistics _stats;
  IdentifierPresentationEntityMap _entities;
  geom::Size _size;
  compositor::PresentationEntity* _root;
  animation::Director _animationDirector;
  layout::Solver _layoutSolver;
  bool _hasVisualUpdates;
  layout::ProxyResolver _proxyResolver;

  void updateRootEntity(compositor::PresentationEntity* entity);

  void updateRootSizeSuggestions();

  compositor::PresentationEntity& presentationEntityForName(
      const core::Name& name);

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

  layout::Solver::FlushResult resolveConstraintUpdate(
      const layout::Variable& variable,
      double value);

  double resolveConstraintConstant(const layout::Variable& variable) const;

  void syncSolverStats();

  RL_DISALLOW_COPY_AND_ASSIGN(PresentationGraph);
};

}  // namespace coordinator
}  // namespace rl

#endif  // RADARLOVE_COORDINATOR_PRESENTATIONGRAPH_H_
