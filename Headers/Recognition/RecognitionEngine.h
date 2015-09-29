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

  bool applyTouchMap(TouchEvent::PhaseMap&& map);

 private:
  std::set<GestureRecognizer, GestureRecognizer::Less> _recognizers;
  GestureRecognizer::ObservedEntities _observedEntities;
  GestureRecognizer::ObservedEntities _affectedEntities;

  RL_DISALLOW_COPY_AND_ASSIGN(RecognitionEngine);
};

}  // namespace rl

#endif  // RADARLOVE_RECOGNITION_RECOGNITIONENGINE_H_
