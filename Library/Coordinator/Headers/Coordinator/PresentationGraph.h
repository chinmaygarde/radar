/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Animation/Action.h>
#include <Animation/Director.h>
#include <Animation/Interpolator.h>
#include <Compositor/FrontendPass.h>
#include <Compositor/InterfaceStatistics.h>
#include <Compositor/PresentationEntity.h>
#include <Coordinator/TransactionPayload.h>
#include <Coordinator/TransferEntity.h>
#include <Core/Macros.h>
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

  void presentStatistics() const;

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

  void onEditVariableUpdate(const expr::Variable& variable, bool addOrRemove);

  void onEditVariableSuggest(const expr::Variable& variable, double value);

  layout::Solver::FlushResult resolveConstraintUpdate(
      const expr::Variable& variable,
      double value);

  double resolveConstraintConstant(const expr::Variable& variable) const;

  void syncSolverStats();

  RL_DISALLOW_COPY_AND_ASSIGN(PresentationGraph);
};

}  // namespace coordinator
}  // namespace rl
