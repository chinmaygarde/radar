/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <functional>
#include <map>
#include <set>
#include <vector>

namespace rl {
namespace toolbox {

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
  StateMachine(std::vector<LegalTransition> legalTransitions);

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
  std::map<State, std::set<State>> _legalTransitions;
  std::map<std::pair<State, State>, Callback> _transitionCallbacks;

  bool performSingleTransition(State state);
  std::vector<State> pathToState(State state);
  void stepTransitionNoCheck(State state);

  RL_DISALLOW_COPY_AND_ASSIGN(StateMachine);
};

}  // namespace toolbox
}  // namespace rl
