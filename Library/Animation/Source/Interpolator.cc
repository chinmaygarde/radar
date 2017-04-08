/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Animation/Interpolator.h>
#include <Entity/Color.h>

namespace rl {
namespace animation {

template <class T>
Interpolator<T>::Interpolator(const Action& action,
                              const T& from,
                              const T& to,
                              Stepper stepper)
    : _action(action),
      _from(from),
      _to(to),
      _stepper(stepper),
      _start(core::ClockPoint::min()) {}

template <class T>
void Interpolator<T>::start(const core::ClockPoint& time) {
  _start = time;
}

template <class T>
T Lerp(const T& from, const T& to, double time);

template <class T>
void Interpolator<T>::step(const core::ClockPoint& time) {
  auto timeSinceStart = time - _start;
  auto timeInUnitSlice = _action.unitInterpolation(timeSinceStart);
  auto interpolatedValue = Lerp(_from, _to, timeInUnitSlice);
  _stepper(interpolatedValue);
}

template <class T>
const T& Interpolator<T>::from() const {
  return _from;
}

template <class T>
const T& Interpolator<T>::to() const {
  return _to;
}

/*
 *  ============================================================================
 *  Interpolating individual typess.
 *  ============================================================================
 */

template <>
double Lerp(const double& from, const double& to, double time) {
  return from + (to - from) * time;
}

template <>
geom::Point Lerp(const geom::Point& from, const geom::Point& to, double time) {
  return {
      Lerp(from.x, to.x, time),  // x
      Lerp(from.y, to.y, time)   // y
  };
}

template <>
geom::Size Lerp(const geom::Size& from, const geom::Size& to, double time) {
  return {
      Lerp(from.width, to.width, time),   // width
      Lerp(from.height, to.height, time)  // height
  };
}

template <>
geom::Rect Lerp(const geom::Rect& from, const geom::Rect& to, double time) {
  return {
      Lerp(from.origin, to.origin, time),  // origin
      Lerp(from.size, to.size, time)       // size
  };
}

template <>
geom::Vector3 Lerp(const geom::Vector3& from,
                   const geom::Vector3& to,
                   double time) {
  return {
      Lerp(from.x, to.x, time),  // x
      Lerp(from.y, to.y, time),  // y
      Lerp(from.z, to.z, time),  // z
  };
}

template <>
geom::Vector4 Lerp(const geom::Vector4& from,
                   const geom::Vector4& to,
                   double time) {
  return {
      Lerp(from.x, to.x, time),  // x
      Lerp(from.y, to.y, time),  // y
      Lerp(from.z, to.z, time),  // z
      Lerp(from.w, to.w, time),  // w
  };
}

template <>
geom::Shear Lerp(const geom::Shear& from, const geom::Shear& to, double time) {
  return {
      Lerp(from.xy, to.xy, time),  // xy
      Lerp(from.xz, to.xz, time),  // xz
      Lerp(from.yz, to.yz, time),  // yz
  };
}

template <>
geom::Matrix::Decomposition Lerp(const geom::Matrix::Decomposition& from,
                                 const geom::Matrix::Decomposition& to,
                                 double time) {
  return {
      Lerp(from.translation, to.translation, time),  // translation
      Lerp(from.scale, to.scale, time),              // scale
      Lerp(from.shear, to.shear, time),              // shear
      Lerp(from.perspective, to.perspective, time),  // perspective
      from.rotation.slerp(to.rotation, time),        // rotation
  };
}

template <>
entity::ColorHSB Lerp(const entity::ColorHSB& from,
                      const entity::ColorHSB& to,
                      double time) {
  return {
      Lerp(from.hue, to.hue, time),                // hue
      Lerp(from.saturation, to.saturation, time),  // saturation
      Lerp(from.brightness, to.brightness, time),  // brightness
      Lerp(from.alpha, to.alpha, time),            // alpha
  };
}

/*
 * Explicit Template Specializations.
 */
template class Interpolator<double>;
template class Interpolator<geom::Point>;
template class Interpolator<geom::Size>;
template class Interpolator<geom::Rect>;
template class Interpolator<geom::Matrix::Decomposition>;
template class Interpolator<entity::ColorHSB>;

}  // namespace animation
}  // namespace rl
