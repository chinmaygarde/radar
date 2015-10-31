// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Recognition/ActiveTouchSet.h>

#include <algorithm>

namespace rl {
namespace recognition {

ActiveTouchSet::ActiveTouchSet() {
}

ActiveTouchSet::~ActiveTouchSet() {
}

size_t ActiveTouchSet::size() const {
  return _activeTouches.size();
}

void ActiveTouchSet::add(const std::vector<event::TouchEvent>& touches) {
  for (const auto& touch : touches) {
    auto identifier = touch.identifier();
    auto touchEntity = core::make_unique<TouchEntity>(touch);

    auto identifierTouchPair =
        std::make_pair(identifier, std::move(touchEntity));

    auto res = _activeTouches.insert(std::move(identifierTouchPair));
    RL_ASSERT_MSG(res.second,
                  "A touch that was already active was added again");

    _indexedTouches.push_back(identifier);
  }
}

void ActiveTouchSet::update(const std::vector<event::TouchEvent>& touches) {
  for (const auto& touch : touches) {
    auto& entity = _activeTouches.at(touch.identifier());
    RL_ASSERT(entity != nullptr);
    /*
     *  These are touch entities, so no view matrix conversions are necessary
     */
    entity->setPosition(touch.location());
  }
}

void ActiveTouchSet::clear(const std::vector<event::TouchEvent>& touches) {
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
  const auto entity = touchEntityForIndex(index);
  if (entity) {
    return PointResult(true, entity->position());
  }

  return PointResult(false, geom::PointZero);
}

TouchEntity* ActiveTouchSet::touchEntityForProxy(Variable::Proxy proxy) const {
  return touchEntityForIndex(static_cast<size_t>(proxy));
}

TouchEntity* ActiveTouchSet::touchEntityForIndex(size_t index) const {
  if (index + 1 > _activeTouches.size()) {
    return nullptr;
  }

  const auto& touchEvent = _activeTouches.at(_indexedTouches[index]);
  /*
   *  Paranoid assertion since bounds checking has already been done
   */
  RL_ASSERT(touchEvent != nullptr);

  return touchEvent.get();
}

}  // namespace recognition
}  // namespace rl
