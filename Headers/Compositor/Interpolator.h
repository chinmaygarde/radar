// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_COMPOSITOR_INTERPOLATOR_
#define RADARLOVE_COMPOSITOR_INTERPOLATOR_

#include <Core/Core.h>
#include <Interface/Action.h>
#include <Compositor/PresentationEntity.h>

namespace rl {
template <typename Type>
class Interpolator {
 public:
  Interpolator(PresentationEntity::Borrowed entity,
               const Action& action,
               const typename Entity::Accessors<Type>::Setter& setter,
               const Type& from,
               const Type& to);

  const Type& from() const;

  const Type& to() const;

  void start(const ClockPoint& time);

  void step(const ClockPoint& time);

 private:
  PresentationEntity::Borrowed _entity;
  const Action _action;
  const typename Entity::Accessors<Type>::Setter _setter;
  Type _from;
  Type _to;
  ClockPoint _start;

  Type x(double unitTime) const;

  RL_DISALLOW_COPY_AND_ASSIGN(Interpolator);
};
}  // namespace rl

#endif  // RADARLOVE_COMPOSITOR_INTERPOLATOR_
