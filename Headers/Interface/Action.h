// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_INTERFACE_ACTION__
#define __RADARLOVE_INTERFACE_ACTION__

#include <Core/Core.h>
#include <Compositor/TimingCurve.h>

namespace rl {
class Action : public Serializable {
 public:
  /**
   *  Create a new action specification
   */
  explicit Action(double duration = 0.125);

  /**
   *  Get the total duration of a single iteration of the action. The default is
   *  0.125
   *
   *  @return the action duration
   */
  double duration() const;

  /**
   *  Set the duration of a single iteration of the action
   *
   *  @param duration the action duration
   */
  void setDuration(double duration);

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

  /**
   *  Get the initial begin time offset of the action. The default is 0.
   *
   *  @return the begin time offset
   */
  double beginTime() const;

  /**
   *  Set the begin time offset of the animation
   */
  void setBeginTime(double beginTime);

  uint64_t propertyMask() const;

  void setPropertyMask(uint64_t mask);

  TimingCurve::Type timingCurveType() const;

  void setTimingCurveType(TimingCurve::Type type);

  virtual bool serialize(Message& message) const override;

  virtual bool deserialize(Message& message) override;

 private:
  double _duration;
  uint64_t _repeatCount;
  bool _autoReverses;
  double _beginTime;
  uint64_t _propertyMask;
  TimingCurve::Type _timingCurveType;

  DISALLOW_ASSIGN(Action);
};
}

#endif /* defined(__RADARLOVE_INTERFACE_ACTION__) */
