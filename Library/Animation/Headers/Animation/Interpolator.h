/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Animation/Action.h>
#include <Core/Macros.h>

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

  RL_DISALLOW_COPY_AND_ASSIGN(Interpolator);
};

}  // namespace animation
}  // namespace rl
