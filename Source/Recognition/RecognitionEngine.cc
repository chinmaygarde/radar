// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Recognition/RecognitionEngine.h>

namespace rl {
namespace recognition {

RecognitionEngine::RecognitionEngine() {
}

RecognitionEngine::~RecognitionEngine() {
}

void RecognitionEngine::setupRecognizers(
    GestureRecognizer::Collection&& recognizers) {
  for (auto& recognizer : recognizers) {
    RL_ASSERT(recognizer.isSolvable());
    const auto identifier = recognizer.identifier();
    auto res = _recognizers.emplace(identifier, std::move(recognizer));
    RL_ASSERT(res.second);
  }
}

RecognitionEngine::Result RecognitionEngine::processAddedTouches(
    const std::vector<event::TouchEvent>& touches,
    const coordinator::PresentationEntity::IdentifierMap& entities) {
  RL_ASSERT(touches.size() != 0);

  /*
   *  Add the new touches to the set of touches considered active
   */
  _activeTouches.add(touches);

  /*
   *  Ask available gesture recognizers if the new set of active touches
   *  satisfy their active conditions
   */
  Result result = Result::NoUpdates;

  for (const auto& item : _recognizers) {
    if (item.second.shouldBeginRecognition(_activeTouches, entities)) {
      _activeRecognizers.insert(item.second.identifier());
      result = Result::Updated;
    }
  }

  return result;
}

RecognitionEngine::Result RecognitionEngine::stepActiveRecognizers(
    const coordinator::PresentationEntity::IdentifierMap& entities) {
  Result result = Result::NoUpdates;

  GestureRecognizer::Collection ended;
  GestureRecognizer::Collection cancelled;

  std::set<GestureRecognizer::Identifier> expiredIdentifiers;

  for (const auto& activeRecognizerID : _activeRecognizers) {
    const auto& recognizer = _recognizers.at(activeRecognizerID);
    switch (recognizer.stepRecognition(_activeTouches, entities)) {
      case GestureRecognizer::Continuation::Continue:
        break;
      case GestureRecognizer::Continuation::End:
      case GestureRecognizer::Continuation::Cancel:
        expiredIdentifiers.insert(activeRecognizerID);
        break;
    }

    result = Result::Updated;
  }

  for (const auto& expired : expiredIdentifiers) {
    _activeRecognizers.erase(expired);
  }

  return result;
}

RecognitionEngine::Result RecognitionEngine::processMovedTouches(
    const std::vector<event::TouchEvent>& touches,
    const coordinator::PresentationEntity::IdentifierMap& entities) {
  RL_ASSERT(touches.size() != 0);

  _activeTouches.update(touches);

  return stepActiveRecognizers(entities);
}

RecognitionEngine::Result RecognitionEngine::processEndedTouches(
    const std::vector<event::TouchEvent>& touches,
    const coordinator::PresentationEntity::IdentifierMap& entities) {
  RL_ASSERT(touches.size() != 0);

  _activeTouches.clear(touches);

  return stepActiveRecognizers(entities);
}

RecognitionEngine::Result RecognitionEngine::processCancelledTouches(
    const std::vector<event::TouchEvent>& touches,
    const coordinator::PresentationEntity::IdentifierMap& entities) {
  RL_ASSERT(touches.size() != 0);

  /*
   *  Note:
   *  Cancellation just overrides the results of successful recognition to the
   *  cancelled state. In case of other behavior, it is identical to touch ends.
   */
  _activeTouches.clear(touches);

  return stepActiveRecognizers(entities);
}

bool RecognitionEngine::isEngineConsistent() const {
  if (_activeTouches.size() == 0) {
    if (_activeRecognizers.size() != 0) {
      return false;
    }
  }

  return true;
}

RecognitionEngine::Result RecognitionEngine::applyTouchMap(
    const event::TouchEvent::PhaseMap& map,
    const coordinator::PresentationEntity::IdentifierMap& entities) {
  if (_recognizers.size() == 0) {
    return Result::NoUpdates;
  }

  using Phase = event::TouchEvent::Phase;
  auto updated = false;

  /*
   *  Phase::Began
   */
  auto found = map.find(Phase::Began);
  if (found != map.end()) {
    updated |= processAddedTouches(found->second, entities) == Result::Updated;
  }

  /*
   *  Phase::Moved
   */
  found = map.find(Phase::Moved);
  if (found != map.end()) {
    updated |= processMovedTouches(found->second, entities) == Result::Updated;
  }

  /*
   *  Phase::Ended
   */
  found = map.find(Phase::Ended);
  if (found != map.end()) {
    updated |= processEndedTouches(found->second, entities) == Result::Updated;
  }

  /*
   *  Phase::Cancelled
   */
  found = map.find(Phase::Cancelled);
  if (found != map.end()) {
    updated |=
        processCancelledTouches(found->second, entities) == Result::Updated;
  }

  RL_ASSERT_MSG(isEngineConsistent(),
                "After each touch process pass, the recognition engine must be "
                "consistent");

  return updated ? Result::Updated : Result::NoUpdates;
}

}  // namespace recognition
}  // namespace rl
