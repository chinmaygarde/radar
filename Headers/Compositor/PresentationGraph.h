// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_COMPOSITOR_PRESENTATIONGRAPH_H_
#define RADARLOVE_COMPOSITOR_PRESENTATIONGRAPH_H_

#include <Core/Core.h>
#include <Compositor/EntityArena.h>
#include <Compositor/Frame.h>
#include <Compositor/PresentationEntity.h>
#include <Compositor/TransferRecord.h>
#include <Interface/Action.h>
#include <Compositor/AnimationDirector.h>
#include <Compositor/TransactionPayload.h>
#include <Recognition/RecognitionEngine.h>

#include <map>

namespace rl {

class PresentationGraph {
 public:
  explicit PresentationGraph();
  ~PresentationGraph();

  bool applyTransactions(core::Message& arena);
  bool applyTouchMap(event::TouchEvent::PhaseMap&& map);

  void render(Frame& frame);

  AnimationDirector& animationDirector();

 private:
  PresentationEntity::IdentifierMap _entities;
  RecognitionEngine _recognitionEngine;

  PresentationEntity* _root;
  AnimationDirector _animationDirector;

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
  void onRecognizerCommit(GestureRecognizer::Collection&& recognizers);

  RL_DISALLOW_COPY_AND_ASSIGN(PresentationGraph);
};

}  // namespace rl

#endif  // RADARLOVE_COMPOSITOR_PRESENTATIONGRAPH_H_
