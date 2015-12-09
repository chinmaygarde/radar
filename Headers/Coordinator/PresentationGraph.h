// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_COORDINATOR_PRESENTATIONGRAPH_H_
#define RADARLOVE_COORDINATOR_PRESENTATIONGRAPH_H_

#include <Animation/Director.h>
#include <Coordinator/EntityArena.h>
#include <Coordinator/Frame.h>
#include <Coordinator/PresentationEntity.h>
#include <Coordinator/TransactionPayload.h>
#include <Coordinator/TransferRecord.h>
#include <Core/Core.h>
#include <Interface/Action.h>
#include <Layout/Solver.h>
#include <Layout/Suggestion.h>
#include <Recognition/RecognitionEngine.h>

#include <map>

namespace rl {
namespace coordinator {

class PresentationGraph {
 public:
  explicit PresentationGraph();
  ~PresentationGraph();

  bool applyTransactions(core::Message& arena);

  recognition::RecognitionEngine::Result applyTouchMap(
      const event::TouchEvent::PhaseMap& touches);

  layout::Solver::FlushResult applyConstraints();

  void render(Frame& frame);

  animation::Director& animationDirector();

 private:
  PresentationEntity::IdentifierMap _entities;
  PresentationEntity* _root;
  animation::Director _animationDirector;
  recognition::RecognitionEngine _recognitionEngine;
  layout::Solver _layoutSolver;

  bool applyTransactionSingle(core::Message& arena,
                              const core::ClockPoint& time);

  void prepareActions(interface::Action& action,
                      PresentationEntity& currentState,
                      const TransferRecord& record,
                      const core::ClockPoint& time);

  template <typename T>
  void prepareActionSingle(interface::Action& action,
                           PresentationEntity& entity,
                           const TransferRecord& record,
                           const interface::Entity::Accessors<T>& accessors,
                           const core::ClockPoint& start);

  void onActionCommit(interface::Action& action);

  void onTransferRecordCommit(interface::Action& action,
                              TransferRecord& record,
                              const core::ClockPoint& commitTime);
  void onRecognizerCommit(
      recognition::GestureRecognizer::Collection&& recognizers);

  void onConstraintsCommit(std::vector<layout::Constraint>&& constraints);

  void onSuggestionsCommit(std::vector<layout::Suggestion>&& suggestions);

  void resolveConstraintUpdate(const layout::Variable& variable, double value);

  RL_DISALLOW_COPY_AND_ASSIGN(PresentationGraph);
};

}  // namespace coordinator
}  // namespace rl

#endif  // RADARLOVE_COORDINATOR_PRESENTATIONGRAPH_H_
