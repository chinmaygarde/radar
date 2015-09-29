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

    if (res.second) {
      const auto& inserted = (*res.first);
      const auto& observed = inserted.observedEntities();
      _observedEntities.insert(observed.begin(), observed.end());
      _affectedEntities.insert(inserted.affectedEntity());
    }
  }
}

bool RecognitionEngine::applyTouchMap(TouchEvent::PhaseMap&& map) {
  return false;
}

}  // namespace rl
