// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Recognition/RecognitionEngine.h>

namespace rl {

RecognitionEngine::RecognitionEngine() {
}

RecognitionEngine::~RecognitionEngine() {
}

void RecognitionEngine::setupRecognizers(
    GestureRecognizer::Collection&& recognizers) {
  for (auto& recognizer : recognizers) {
    const auto identifier = recognizer.identifier();
    auto res = _recognizers.emplace(identifier, std::move(recognizer));
    RL_ASSERT(res.second);
  }
}

void RecognitionEngine::processAddedTouches(
    const std::vector<TouchEvent>& touches,
    const PresentationEntity::IdentifierMap& entities) {
  if (touches.size() == 0) {
    return;
  }

  /*
   *  Add the new touches to the set of touches considered active
   */
  _activeTouches.add(touches);

  /*
   *  Ask available gesture recognizers if the new set of active touches
   *  satisfy their active conditions
   */
  for (const auto& item : _recognizers) {
    if (item.second.shouldBeginRecognition(_activeTouches, entities)) {
      _activeRecognizers.insert(item.second.identifier());
    }
  }
}

void RecognitionEngine::processMovedTouches(
    const std::vector<TouchEvent>& touches,
    const PresentationEntity::IdentifierMap& entities) {
  if (touches.size() == 0) {
    return;
  }
}

void RecognitionEngine::processEndedTouches(
    const std::vector<TouchEvent>& touches,
    const PresentationEntity::IdentifierMap& entities) {
  if (touches.size() == 0) {
    return;
  }

  _activeTouches.clear(touches);

  GestureRecognizer::Collection ended;
  GestureRecognizer::Collection cancelled;

  for (const auto& activeRecognizerID : _activeRecognizers) {
    const auto& recognizer = _recognizers.at(activeRecognizerID);
    switch (recognizer.shouldContinueRecognition(_activeTouches, entities)) {
      case GestureRecognizer::Continuation::Continue:
        break;
      case GestureRecognizer::Continuation::End:
        break;
      case GestureRecognizer::Continuation::Cancel:
        break;
    }
  }
}

void RecognitionEngine::processCancelledTouches(
    const std::vector<TouchEvent>& touches,
    const PresentationEntity::IdentifierMap& entities) {
  if (touches.size() == 0) {
    return;
  }

  /*
   *  FIXME: Cancellation of a single touch cancels all active recognizers
   */
  _activeRecognizers.clear();

  _activeTouches.clear(touches);
}

bool RecognitionEngine::isEngineConsistent() const {
  if (_activeTouches.size() == 0) {
    if (_activeRecognizers.size() != 0) {
      return false;
    }
  }

  return true;
}

bool RecognitionEngine::applyTouchMap(
    TouchEvent::PhaseMap&& map,
    const PresentationEntity::IdentifierMap& entities) {
  using Phase = TouchEvent::Phase;

  processAddedTouches(map[Phase::Began], entities);
  processMovedTouches(map[Phase::Moved], entities);
  processEndedTouches(map[Phase::Ended], entities);
  processCancelledTouches(map[Phase::Cancelled], entities);

  RL_ASSERT(isEngineConsistent() &&
            "After each touch process pass, the recognition engine must be "
            "consistent");

  return true;
}

}  // namespace rl
