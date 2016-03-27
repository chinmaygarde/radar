// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_ANIMATION_INTERPOLATOR_
#define RADARLOVE_ANIMATION_INTERPOLATOR_

#include <Core/Core.h>
#include <Animation/Action.h>

namespace rl {
namespace animation {

template <typename Type>
class Interpolator {
 public:
  using Stepper = std::function<void(const Type& /* value */)>;
  Interpolator(const Action& action,
               const Type& from,
               const Type& to,
               Stepper stepper);

  const Type& from() const;

  const Type& to() const;

  void start(const core::ClockPoint& time);

  void step(const core::ClockPoint& time);

 private:
  const Action _action;
  Type _from;
  Type _to;
  const Stepper _stepper;
  core::ClockPoint _start;

  Type x(double unitTime) const;

  RL_DISALLOW_COPY_AND_ASSIGN(Interpolator);
};

}  // namespace animation
}  // namespace rl

#endif  // RADARLOVE_ANIMATION_INTERPOLATOR_
