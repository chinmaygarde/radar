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
  using EntityMap =
      std::map<Entity::Identifier, std::unique_ptr<PresentationEntity>>;

  explicit PresentationGraph();
  ~PresentationGraph();

  bool applyTransactions(Message& arena);
  bool applyTouchMap(TouchEvent::PhaseMap&& map);

  void render(Frame& frame);

  AnimationDirector& animationDirector();

 private:
  EntityMap _entities;
  RecognitionEngine _recognitionEngine;

  PresentationEntity* _root;
  AnimationDirector _animationDirector;

  bool applyTransactionSingle(Message& arena, const ClockPoint& time);
  void prepareActions(Action& action,
                      PresentationEntity& currentState,
                      const TransferRecord& record,
                      const ClockPoint& time);
  template <typename T>
  void prepareActionSingle(Action& action,
                           PresentationEntity& entity,
                           const TransferRecord& record,
                           const Entity::Accessors<T>& accessors,
                           const ClockPoint& start);

  void onActionCommit(Action& action);
  void onTransferRecordCommit(Action& action,
                              TransferRecord& record,
                              const ClockPoint& commitTime);
  void onRecognizerCommit(GestureRecognizer::Collection&& recognizers);

  RL_DISALLOW_COPY_AND_ASSIGN(PresentationGraph);
};

}  // namespace rl

#endif  // RADARLOVE_COMPOSITOR_PRESENTATIONGRAPH_H_
