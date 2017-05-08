/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Animation/TimingCurve.h>
#include <Core/Macros.h>
#include <Entity/Entity.h>

namespace rl {
namespace animation {

class Action : public core::ArchiveSerializable,
               public core::MessageSerializable {
 public:
  /**
   *  Create a new action specification
   */
  Action(double duration = 0.125);

  /**
   *  Get the total duration of a single iteration of the action. The default is
   *  0.125
   *
   *  @return the action duration
   */
  const core::ClockDuration& duration() const;

  /**
   *  Set the duration of a single iteration of the action
   *
   *  @param duration the action duration
   */
  void setDuration(const core::ClockDuration& duration);

  /**
   *  Get the repeat count of the action. The default is 1
   *
   *  @return the action repeat count
   */
  uint64_t repeatCount() const;

  static const uint64_t RepeatCountInfinity =
      std::numeric_limits<uint64_t>::max();

  /**
   *  Set the repeat count of the action
   *
   *  @param repeatCount the new repeat count
   */
  void setRepeatCount(uint64_t repeatCount);

  /**
   *  If the action reverses between alternate repetitions. Repeat count must
   *  be greater than 1 for this to take effect. The default is false
   *
   *  @return if the action reverses
   */
  bool autoReverses() const;

  /**
   *  Set if the action reverses between alternate repetitions
   *
   *  @param autoReverses if the action reverses
   */
  void setAutoReverses(bool autoReverses);

  entity::Entity::PropertyMaskType propertyMask() const;

  void setPropertyMask(entity::Entity::PropertyMaskType mask);

  animation::TimingCurve::Type timingCurveType() const;

  void setTimingCurveType(animation::TimingCurve::Type type);

  double unitInterpolation(const core::ClockDuration& time) const;

  bool serialize(core::Message& message) const override;

  bool deserialize(core::Message& message, core::Namespace* ns) override;

  static const core::ArchiveDef ArchiveDefinition;

  ArchiveName archiveName() const override;

  bool serialize(core::ArchiveItem& item) const override;

  bool deserialize(core::ArchiveItem& item, core::Namespace* ns) override;

 private:
  core::ClockDuration _duration;
  uint64_t _repeatCount;
  bool _autoReverses;
  entity::Entity::PropertyMaskType _propertyMask;
  TimingCurve::Type _timingCurveType;
  TimingCurve _resolvedCurve;

  void resolveCurve();

  RL_DISALLOW_ASSIGN(Action);
};

}  // namespace animation
}  // namespace rl
