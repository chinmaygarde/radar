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
    auto res = _recognizers.emplace(std::move(recognizer));
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
  for (const auto& recognizer : _recognizers) {
    if (recognizer.shouldBeginRecognition(_activeTouches, entities)) {
      _activeRecognizers.insert(recognizer.identifier());
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
}

void RecognitionEngine::processCancelledTouches(
    const std::vector<TouchEvent>& touches,
    const PresentationEntity::IdentifierMap& entities) {
  if (touches.size() == 0) {
    return;
  }

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
  processAddedTouches(map[TouchEvent::Phase::Began], entities);
  processMovedTouches(map[TouchEvent::Phase::Moved], entities);
  processEndedTouches(map[TouchEvent::Phase::Ended], entities);
  processCancelledTouches(map[TouchEvent::Phase::Cancelled], entities);

  RL_ASSERT(isEngineConsistent() &&
            "After each touch process pass, the recognition engine must be "
            "consistent");

  return true;
}

}  // namespace rl
