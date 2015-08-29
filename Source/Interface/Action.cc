// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Interface/Action.h>

namespace rl {

Action::Action(double duration)
    : _duration(duration),
      _repeatCount(1),
      _autoReverses(false),
      _beginTime(0.0),
      _propertyMask(0) {
}

double Action::duration() const {
  return _duration;
}

void Action::setDuration(double duration) {
  _duration = duration;
}

uint64_t Action::repeatCount() const {
  return _repeatCount;
}

void Action::setRepeatCount(uint64_t repeatCount) {
  _repeatCount = repeatCount;
}

bool Action::autoReverses() const {
  return _autoReverses;
}

void Action::setAutoReverses(bool autoReverses) {
  _autoReverses = autoReverses;
}

double Action::beginTime() const {
  return _beginTime;
}

void Action::setBeginTime(double beginTime) {
  _beginTime = beginTime;
}

uint64_t Action::propertyMask() const {
  return _propertyMask;
}

void Action::setPropertyMask(uint64_t mask) {
  _propertyMask = mask;
}

void Action::serialize(Message& message) const {
  message.encode(_duration);
  message.encode(_repeatCount);
  message.encode(_autoReverses);
  message.encode(_beginTime);
  message.encode(_propertyMask);
}

void Action::deserialize(Message& message) {
  message.decode(_duration);
  message.decode(_repeatCount);
  message.decode(_autoReverses);
  message.decode(_beginTime);
  message.decode(_propertyMask);
}

}  // namespace rl
