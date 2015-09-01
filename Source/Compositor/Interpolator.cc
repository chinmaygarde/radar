// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/Interpolator.h>
#include <Compositor/Color.h>

namespace rl {

template <typename Type>
Interpolator<Type>::Interpolator(
    PresentationEntity::Borrowed entity,
    const Action& action,
    const typename Entity::Accessors<Type>::Setter& setter,
    const Type& from,
    const Type& to)
    : _entity(entity),
      _action(action),
      _setter(setter),
      _from(from),
      _to(to),
      _start(0) {
}

template <typename Type>
void Interpolator<Type>::start(std::chrono::nanoseconds time) {
  _start = time;
}

template <typename Type>
Type Interpolator<Type>::x(std::chrono::nanoseconds time) const {
  return _from;
}

template <typename Type>
Type Interpolator<Type>::from() const {
  return _from;
}

template <typename Type>
Type Interpolator<Type>::to() const {
  return _to;
}

/**
 * Explicit Template Specializations
 */
template class Interpolator<double>;
template class Interpolator<Point>;
template class Interpolator<Size>;
template class Interpolator<Rect>;
template class Interpolator<Color>;
template class Interpolator<Matrix>;

}  // namespace rl
