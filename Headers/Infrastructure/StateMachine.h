// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_STATE_MACHINE_
#define RADARLOVE_STATE_MACHINE_

#include <Core/Core.h>

#include <functional>
#include <vector>
#include <map>
#include <set>

namespace rl {
class StateMachine {
 public:
  using State = uint64_t;
  using Callback = std::function<void()>;
  using LegalTransition =
      std::tuple<State /* from */, State /* to */, Callback>;

  /**
   *  Construct a state machine given the set of legal state transitions along
   *  with their callbacks
   */
  explicit StateMachine(std::vector<LegalTransition> legalTransitions);

  /**
   *  Get the current state of the state machine
   *
   *  @return the current state
   */
  State state() const;

  /**
   *  Attempt to update the state of the state machine
   *
   *  @param state if the state was updated
   */
  bool setState(State state, bool allowMultiple = false);

 private:
  State _currentState;
  bool _activated;
  std::map<State, std::set<State>> _legalTransitions;
  std::map<std::pair<State, State>, Callback> _transitionCallbacks;

  bool performSingleTransition(State state);
  std::vector<State> pathToState(State state);
  void stepTransitionNoCheck(State state);

  RL_DISALLOW_COPY_AND_ASSIGN(StateMachine);
};
}

#endif  // RADARLOVE_STATE_MACHINE_
