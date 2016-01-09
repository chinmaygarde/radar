// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Interface/Action.h>

namespace rl {
namespace interface {

Action::Action(double duration)
    : _duration(duration),
      _repeatCount(1),
      _autoReverses(false),
      _propertyMask(0),
      _timingCurveType(animation::TimingCurve::Linear),
      _resolvedCurve(
          animation::TimingCurve::SystemTimingCurve(_timingCurveType)) {}

const core::ClockDuration& Action::duration() const {
  return _duration;
}

void Action::setDuration(const core::ClockDuration& duration) {
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

Entity::PropertyMaskType Action::propertyMask() const {
  return _propertyMask;
}

void Action::setPropertyMask(Entity::PropertyMaskType mask) {
  _propertyMask = mask;
}

animation::TimingCurve::Type Action::timingCurveType() const {
  return _timingCurveType;
}

void Action::setTimingCurveType(animation::TimingCurve::Type type) {
  _timingCurveType = type;
  resolveCurve();
}

void Action::resolveCurve() {
  _resolvedCurve = animation::TimingCurve::SystemTimingCurve(_timingCurveType);
}

double Action::unitInterpolation(const core::ClockDuration& time) const {
  auto unitInterpolation =
      fmod(time.count(), _duration.count()) / _duration.count();

  if (_autoReverses && (static_cast<int>(time / _duration) % 2) == 1) {
    unitInterpolation = 1.0 - unitInterpolation;
  }

  return _resolvedCurve.x(unitInterpolation);
}

bool Action::serialize(core::Message& message) const {
  bool result = true;
  result &= message.encode(_duration);
  result &= message.encode(_repeatCount);
  result &= message.encode(_autoReverses);
  result &= message.encode(_propertyMask);
  result &= message.encode(_timingCurveType);
  return result;
}

bool Action::deserialize(core::Message& message, core::Namespace* ns) {
  bool result = true;
  result &= message.decode(_duration, ns);
  result &= message.decode(_repeatCount, ns);
  result &= message.decode(_autoReverses, ns);
  result &= message.decode(_propertyMask, ns);
  result &= message.decode(_timingCurveType, ns);
  resolveCurve();
  return result;
}

}  // namespace interface
}  // namespace rl
