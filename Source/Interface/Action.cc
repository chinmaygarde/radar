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
      _timingCurveType(animation::TimingCurve::Type::Linear),
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
  auto result = true;
  result &= message.encode(_duration);
  result &= message.encode(_repeatCount);
  result &= message.encode(_autoReverses);
  result &= message.encode(_propertyMask);
  result &= message.encode(_timingCurveType);
  return result;
}

bool Action::deserialize(core::Message& message, core::Namespace* ns) {
  auto result = true;
  result &= message.decode(_duration, ns);
  result &= message.decode(_repeatCount, ns);
  result &= message.decode(_autoReverses, ns);
  result &= message.decode(_propertyMask, ns);
  result &= message.decode(_timingCurveType, ns);
  resolveCurve();
  return result;
}

enum ActionArchiveKey {
  Duration,
  RepeatCount,
  AutoReverses,
  PropertyMask,
  TimingCurveType,
};

const core::ArchiveDef Action::ArchiveDefinition = {
    .className = "Action",
    .autoAssignName = true,
    .members = {
        ActionArchiveKey::Duration,        //
        ActionArchiveKey::RepeatCount,     //
        ActionArchiveKey::AutoReverses,    //
        ActionArchiveKey::PropertyMask,    //
        ActionArchiveKey::TimingCurveType  //
    }};

Action::ArchiveName Action::archiveName() const {
  return core::ArchiveNameAuto;
}

bool Action::serialize(core::ArchiveItem& item) const {
  auto curve = static_cast<animation::TimingCurve::Data>(_timingCurveType);

  using K = ActionArchiveKey;

  auto result = true;
  result &= item.encode(K::Duration, _duration.count());
  result &= item.encode(K::RepeatCount, _repeatCount);
  result &= item.encode(K::AutoReverses, _autoReverses);
  result &= item.encode(K::PropertyMask, _propertyMask);
  result &= item.encode(K::TimingCurveType, curve);
  return result;
}

bool Action::deserialize(core::ArchiveItem& item) {
  animation::TimingCurve::Data curve = 0;
  core::ClockDuration::rep durationRep = 0;

  using K = ActionArchiveKey;

  auto result = true;

  result &= item.decode(K::Duration, durationRep);
  result &= item.decode(K::RepeatCount, _repeatCount);
  result &= item.decode(K::AutoReverses, _autoReverses);
  result &= item.decode(K::PropertyMask, _propertyMask);
  result &= item.decode(K::TimingCurveType, curve);

  _timingCurveType = static_cast<animation::TimingCurve::Type>(curve);
  _duration = core::ClockDuration(durationRep);

  return result;
}

}  // namespace interface
}  // namespace rl
