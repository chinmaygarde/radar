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

void RecognitionEngine::processAddedTouches(
    const std::vector<event::TouchEvent>& touches,
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

void RecognitionEngine::stepActiveRecognizers(
    const PresentationEntity::IdentifierMap& entities) {
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
  }

  for (const auto& expired : expiredIdentifiers) {
    _activeRecognizers.erase(expired);
  }
}

void RecognitionEngine::processMovedTouches(
    const std::vector<event::TouchEvent>& touches,
    const PresentationEntity::IdentifierMap& entities) {
  if (touches.size() == 0) {
    return;
  }

  _activeTouches.update(touches);
  stepActiveRecognizers(entities);
}

void RecognitionEngine::processEndedTouches(
    const std::vector<event::TouchEvent>& touches,
    const PresentationEntity::IdentifierMap& entities) {
  if (touches.size() == 0) {
    return;
  }

  _activeTouches.clear(touches);
  stepActiveRecognizers(entities);
}

void RecognitionEngine::processCancelledTouches(
    const std::vector<event::TouchEvent>& touches,
    const PresentationEntity::IdentifierMap& entities) {
  if (touches.size() == 0) {
    return;
  }

  /*
   *  Note:
   *  Cancellation just overrides the results of successful recognition to the
   *  cancelled state. In case of other behavior, it is identical to touch ends.
   */
  _activeTouches.clear(touches);
  stepActiveRecognizers(entities);
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
    event::TouchEvent::PhaseMap&& map,
    const PresentationEntity::IdentifierMap& entities) {
  if (_recognizers.size() == 0) {
    return true;
  }

  using Phase = event::TouchEvent::Phase;

  processAddedTouches(map[Phase::Began], entities);
  processMovedTouches(map[Phase::Moved], entities);
  processEndedTouches(map[Phase::Ended], entities);
  processCancelledTouches(map[Phase::Cancelled], entities);

  RL_ASSERT_MSG(isEngineConsistent(),
                "After each touch process pass, the recognition engine must be "
                "consistent");

  return true;
}

}  // namespace recognition
}  // namespace rl
