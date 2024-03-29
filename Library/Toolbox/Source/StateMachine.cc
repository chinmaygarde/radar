/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Toolbox/StateMachine.h>
#include <queue>

namespace rl {
namespace toolbox {

StateMachine::StateMachine(std::vector<LegalTransition> legalTransitions)
    : _currentState(0) {
  for (const auto& transition : legalTransitions) {
    State from, to;
    Callback callback;
    std::tie(from, to, callback) = transition;

    _legalTransitions[from].insert(to);
    _transitionCallbacks[std::make_pair(from, to)] = callback;
  }
}

StateMachine::State StateMachine::state() const {
  return _currentState;
}

bool StateMachine::setState(State state, bool allowMultiple) {
  if (!allowMultiple) {
    return performSingleTransition(state);
  } else {
    std::vector<State> path = pathToState(state);

    if (path.size() < 1) {
      return false;
    }

    RL_ASSERT(path[0] == _currentState);
    const auto size = path.size();

    for (size_t i = 1; i < size; i++) {
      stepTransitionNoCheck(path[i]);
    }
  }

  return false;
}

std::vector<StateMachine::State> StateMachine::pathToState(State final) {
  std::queue<std::vector<State>> queue;
  queue.push({_currentState});
  std::set<State> visited;

  while (queue.size() != 0) {
    std::vector<State> path = queue.front();
    queue.pop();

    State vertex = path.back();

    if (vertex == final) {
      return path;
    } else if (visited.find(vertex) == visited.end()) {
      for (const auto& next : _legalTransitions[vertex]) {
        std::vector<State> alt(path);
        alt.push_back(next);
        queue.push(alt);
      }
      visited.insert(vertex);
    }
  }

  return {_currentState};
}

bool StateMachine::performSingleTransition(State state) {
  auto validTransitions = _legalTransitions[_currentState];

  if (validTransitions.find(state) == validTransitions.end()) {
    /*
     *  Not a valid transition
     */
    return false;
  }

  stepTransitionNoCheck(state);

  return true;
}

void StateMachine::stepTransitionNoCheck(State state) {
  auto callback = _transitionCallbacks[std::make_pair(_currentState, state)];
  _currentState = state;
  RL_ASSERT(callback != nullptr);
  callback();
}

}  // namespace toolbox
}  // namespace rl
