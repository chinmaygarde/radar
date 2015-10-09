// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Recognition/ActiveTouchSet.h>

#include <algorithm>

namespace rl {

ActiveTouchSet::ActiveTouchSet() {
}

ActiveTouchSet::~ActiveTouchSet() {
}

size_t ActiveTouchSet::size() const {
  return _activeTouches.size();
}

void ActiveTouchSet::add(const std::vector<TouchEvent>& touches) {
  for (const auto& touch : touches) {
    auto identifier = touch.identifier();
    auto res = _activeTouches.insert({identifier, touch});
    RL_ASSERT_MSG(res.second,
                  "A touch that was already active was added again");
    _indexedTouches.push_back(identifier);
  }
}

void ActiveTouchSet::clear(const std::vector<TouchEvent>& touches) {
  for (const auto& touch : touches) {
    auto identifier = touch.identifier();
    auto res = _activeTouches.erase(identifier);
    RL_ASSERT_MSG(res != 0, "A touch that was not already active was ended");
    auto found =
        std::find(_indexedTouches.begin(), _indexedTouches.end(), identifier);
    RL_ASSERT(found != _indexedTouches.end());
    _indexedTouches.erase(found);
  }

  RL_ASSERT(_indexedTouches.size() == _activeTouches.size());
}

ActiveTouchSet::PointResult ActiveTouchSet::pointForIndex(size_t index) const {
  if (index + 1 > _activeTouches.size()) {
    return PointResult(false, PointZero);
  }

  const auto& touchEvent = _activeTouches.at(_indexedTouches[index]);
  return PointResult(true, touchEvent.location());
}

}  // namespace rl
