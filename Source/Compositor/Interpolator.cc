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
      _start(ClockPoint::min()) {
}

template <typename Type>
void Interpolator<Type>::start(const ClockPoint& time) {
  _start = time;
}

template <typename Type>
void Interpolator<Type>::step(const ClockPoint& time) {
  auto timeSinceStart = time - _start;
  _setter(*_entity, x(_action.unitInterpolation(timeSinceStart)));
}

template <typename Type>
const Type& Interpolator<Type>::from() const {
  return _from;
}

template <typename Type>
const Type& Interpolator<Type>::to() const {
  return _to;
}

static inline double _lerp(double from, double to, double t) {
  return from + (to - from) * t;
}

template <>
double Interpolator<double>::x(double t) const {
  return _lerp(_from, _to, t);
}

template <>
Point Interpolator<Point>::x(double t) const {
  return Point(_lerp(_from.x, _to.x, t), _lerp(_from.y, _to.y, t));
}

template <>
Size Interpolator<Size>::x(double t) const {
  return Size(_lerp(_from.width, _to.width, t),
              _lerp(_from.height, _to.height, t));
}

template <>
Rect Interpolator<Rect>::x(double t) const {
  return Rect(_lerp(_from.origin.x, _to.origin.x, t),
              _lerp(_from.origin.y, _to.origin.y, t),
              _lerp(_from.size.width, _to.size.width, t),
              _lerp(_from.size.height, _to.size.height, t));
}

template <>
Color Interpolator<Color>::x(double t) const {
  /*
   *  TODO: Create a specialization that stores the from and to values in HSB
   */
  ColorHSB from = ColorHSB::FromRGB(_from);
  ColorHSB to = ColorHSB::FromRGB(_to);
  auto interpolated = ColorHSB{_lerp(from.hue, to.hue, t),
                               _lerp(from.saturation, to.saturation, t),
                               _lerp(from.brightness, to.brightness, t),
                               _lerp(from.alpha, to.alpha, t)};
  return interpolated.ToRGBA();
}

template <>
Matrix Interpolator<Matrix>::x(double t) const {
  /*
   *  TODO: Create a specialization that interpolates naturally. This one is BS.
   *        Will cause skewing and volumetric distortions.
   */
  return Matrix(
      _lerp(_from.m[0], _to.m[0], t), _lerp(_from.m[1], _to.m[1], t),
      _lerp(_from.m[2], _to.m[2], t), _lerp(_from.m[3], _to.m[3], t),
      _lerp(_from.m[4], _to.m[4], t), _lerp(_from.m[5], _to.m[5], t),
      _lerp(_from.m[6], _to.m[6], t), _lerp(_from.m[7], _to.m[7], t),
      _lerp(_from.m[8], _to.m[8], t), _lerp(_from.m[9], _to.m[9], t),
      _lerp(_from.m[10], _to.m[10], t), _lerp(_from.m[11], _to.m[11], t),
      _lerp(_from.m[12], _to.m[12], t), _lerp(_from.m[13], _to.m[13], t),
      _lerp(_from.m[14], _to.m[14], t), _lerp(_from.m[15], _to.m[15], t));
}

/*
 * Explicit Template Specializations
 */
template class Interpolator<double>;
template class Interpolator<Point>;
template class Interpolator<Size>;
template class Interpolator<Rect>;
template class Interpolator<Color>;
template class Interpolator<Matrix>;

}  // namespace rl
