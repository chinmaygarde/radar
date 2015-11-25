// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_RECOGNITION_RECOGNITIONENGINE_H_
#define RADARLOVE_RECOGNITION_RECOGNITIONENGINE_H_

#include <Core/Core.h>
#include <Recognition/GestureRecognizer.h>
#include <Recognition/ActiveTouchSet.h>

#include <map>

namespace rl {
namespace recognition {

class RecognitionEngine {
 public:
  explicit RecognitionEngine();
  ~RecognitionEngine();

  void setupRecognizers(GestureRecognizer::Collection&& recognizers);

  enum class Result {
    NoUpdates,
    Updated,
  };

  Result applyTouchMap(
      const event::TouchEvent::PhaseMap& map,
      const coordinator::PresentationEntity::IdentifierMap& entities);

 private:
  std::map<GestureRecognizer::Identifier, GestureRecognizer> _recognizers;
  std::set<GestureRecognizer::Identifier> _activeRecognizers;
  ActiveTouchSet _activeTouches;

  Result stepActiveRecognizers(
      const coordinator::PresentationEntity::IdentifierMap& entities);
  Result processAddedTouches(
      const std::vector<event::TouchEvent>& touches,
      const coordinator::PresentationEntity::IdentifierMap& entities);
  Result processMovedTouches(
      const std::vector<event::TouchEvent>& touches,
      const coordinator::PresentationEntity::IdentifierMap& entities);
  Result processEndedTouches(
      const std::vector<event::TouchEvent>& touches,
      const coordinator::PresentationEntity::IdentifierMap& entities);
  Result processCancelledTouches(
      const std::vector<event::TouchEvent>& touches,
      const coordinator::PresentationEntity::IdentifierMap& entities);

  bool isEngineConsistent() const;

  RL_DISALLOW_COPY_AND_ASSIGN(RecognitionEngine);
};

}  // namespace recognition
}  // namespace rl

#endif  // RADARLOVE_RECOGNITION_RECOGNITIONENGINE_H_
