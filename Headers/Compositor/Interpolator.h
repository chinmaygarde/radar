// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_COMPOSITOR_INTERPOLATOR__
#define __RADARLOVE_COMPOSITOR_INTERPOLATOR__

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

  Type from() const;

  Type to() const;

  void start(std::chrono::nanoseconds time);

  Type x(std::chrono::nanoseconds time) const;

 private:
  PresentationEntity::Borrowed _entity;
  const Action _action;
  const typename Entity::Accessors<Type>::Setter _setter;
  const Type _from;
  const Type _to;
  std::chrono::nanoseconds _start;

  DISALLOW_COPY_AND_ASSIGN(Interpolator);
};
}

#endif /* defined(__RADARLOVE_COMPOSITOR_INTERPOLATOR__) */
