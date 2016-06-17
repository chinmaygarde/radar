// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Animation/Action.h>

namespace rl {
namespace animation {

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

entity::Entity::PropertyMaskType Action::propertyMask() const {
  return _propertyMask;
}

void Action::setPropertyMask(entity::Entity::PropertyMaskType mask) {
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
  RL_RETURN_IF_FALSE(message.encode(_duration));
  RL_RETURN_IF_FALSE(message.encode(_repeatCount));
  RL_RETURN_IF_FALSE(message.encode(_autoReverses));
  RL_RETURN_IF_FALSE(message.encode(_propertyMask));
  RL_RETURN_IF_FALSE(message.encode(_timingCurveType));

  return true;
}

bool Action::deserialize(core::Message& message, core::Namespace* ns) {
  RL_RETURN_IF_FALSE(message.decode(_duration, ns));
  RL_RETURN_IF_FALSE(message.decode(_repeatCount, ns));
  RL_RETURN_IF_FALSE(message.decode(_autoReverses, ns));
  RL_RETURN_IF_FALSE(message.decode(_propertyMask, ns));
  RL_RETURN_IF_FALSE(message.decode(_timingCurveType, ns));

  resolveCurve();

  return true;
}

enum ActionArchiveKey {
  Duration,
  RepeatCount,
  AutoReverses,
  PropertyMask,
  TimingCurveType,
};

const core::ArchiveDef Action::ArchiveDefinition = {
    /* .superClass = */ nullptr,
    /* .className = */ "Action",
    /* .autoAssignName = */ true,
    /* .members = */ {
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
  using K = ActionArchiveKey;

  RL_RETURN_IF_FALSE(item.encode(K::Duration, _duration.count()));
  RL_RETURN_IF_FALSE(item.encode(K::RepeatCount, _repeatCount));
  RL_RETURN_IF_FALSE(item.encode(K::AutoReverses, _autoReverses));
  RL_RETURN_IF_FALSE(item.encode(K::PropertyMask, _propertyMask));
  RL_RETURN_IF_FALSE(item.encodeEnum(K::TimingCurveType, _timingCurveType));

  return true;
}

bool Action::deserialize(core::ArchiveItem& item, core::Namespace*) {
  core::ClockDuration::rep durationRep = 0;

  using K = ActionArchiveKey;

  RL_RETURN_IF_FALSE(item.decode(K::Duration, durationRep));
  RL_RETURN_IF_FALSE(item.decode(K::RepeatCount, _repeatCount));
  RL_RETURN_IF_FALSE(item.decode(K::AutoReverses, _autoReverses));
  RL_RETURN_IF_FALSE(item.decode(K::PropertyMask, _propertyMask));
  RL_RETURN_IF_FALSE(item.decodeEnum(K::TimingCurveType, _timingCurveType));

  _duration = core::ClockDuration(durationRep);

  resolveCurve();

  return true;
}

}  // namespace animation
}  // namespace rl
