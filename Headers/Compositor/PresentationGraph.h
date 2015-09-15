// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_COMPOSITOR_PRESENTATIONGRAPH_H__
#define __RADARLOVE_COMPOSITOR_PRESENTATIONGRAPH_H__

#include <Core/Core.h>
#include <Compositor/EntityArena.h>
#include <Compositor/Frame.h>
#include <Compositor/PresentationEntity.h>
#include <Compositor/TransferRecord.h>
#include <Interface/Action.h>
#include <Compositor/AnimationDirector.h>

#include <map>

namespace rl {

class PresentationGraph {
 public:
  explicit PresentationGraph();
  ~PresentationGraph();

  bool applyTransactions(Message& arena);

  void render(Frame& frame);

  AnimationDirector& animationDirector();

 private:
  std::map<Entity::Identifier, std::unique_ptr<PresentationEntity>> _entities;
  PresentationEntity* _root;
  AnimationDirector _animationDirector;

  bool applyTransactionSingle(Message& arena,
                              const std::chrono::nanoseconds& time);
  void prepareActions(Action& action,
                      PresentationEntity& currentState,
                      const TransferRecord& record,
                      const std::chrono::nanoseconds& time);
  template <typename T>
  void prepareActionSingle(Action& action,
                           PresentationEntity& entity,
                           const TransferRecord& record,
                           const Entity::Accessors<T>& accessors,
                           const std::chrono::nanoseconds& start);

  RL_DISALLOW_COPY_AND_ASSIGN(PresentationGraph);
};

}  // namespace rl

#endif  // __RADARLOVE_COMPOSITOR_PRESENTATIONGRAPH_H__
