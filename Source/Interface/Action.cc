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
      _propertyMask(0),
      _timingCurveType(TimingCurve::Linear) {
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

TimingCurve::Type Action::timingCurveType() const {
  return _timingCurveType;
}

void Action::setTimingCurveType(TimingCurve::Type type) {
  _timingCurveType = type;
}

bool Action::serialize(Message& message) const {
  bool result = true;
  result &= message.encode(_duration);
  result &= message.encode(_repeatCount);
  result &= message.encode(_autoReverses);
  result &= message.encode(_beginTime);
  result &= message.encode(_propertyMask);
  result &= message.encode(_timingCurveType);
  return result;
}

bool Action::deserialize(Message& message) {
  bool result = true;
  result &= message.decode(_duration);
  result &= message.decode(_repeatCount);
  result &= message.decode(_autoReverses);
  result &= message.decode(_beginTime);
  result &= message.decode(_propertyMask);
  result &= message.decode(_timingCurveType);
  return result;
}

}  // namespace rl
