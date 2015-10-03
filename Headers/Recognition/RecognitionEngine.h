// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_RECOGNITION_RECOGNITIONENGINE_H_
#define RADARLOVE_RECOGNITION_RECOGNITIONENGINE_H_

#include <Core/Core.h>
#include <Recognition/GestureRecognizer.h>
#include <Event/TouchEvent.h>

namespace rl {

class RecognitionEngine {
 public:
  explicit RecognitionEngine();
  ~RecognitionEngine();

  void setupRecognizers(GestureRecognizer::Collection&& recognizers);

  bool applyTouchMap(TouchEvent::PhaseMap&& map,
                     const PresentationEntity::IdentifierMap& entities);

 private:
  std::set<GestureRecognizer, GestureRecognizer::Less> _recognizers;
  std::set<GestureRecognizer::Identifier> _activeRecognizers;
  TouchEvent::IdentifierMap _activeTouches;

  void processAddedTouches(const std::vector<TouchEvent>& touches,
                           const PresentationEntity::IdentifierMap& entities);
  void processMovedTouches(const std::vector<TouchEvent>& touches,
                           const PresentationEntity::IdentifierMap& entities);
  void processEndedTouches(const std::vector<TouchEvent>& touches,
                           const PresentationEntity::IdentifierMap& entities);
  void processCancelledTouches(
      const std::vector<TouchEvent>& touches,
      const PresentationEntity::IdentifierMap& entities);

  void addToActiveTouches(const std::vector<TouchEvent>& touches);
  void clearFromActiveTouches(const std::vector<TouchEvent>& touches);

  bool isEngineConsistent() const;

  RL_DISALLOW_COPY_AND_ASSIGN(RecognitionEngine);
};

}  // namespace rl

#endif  // RADARLOVE_RECOGNITION_RECOGNITIONENGINE_H_
