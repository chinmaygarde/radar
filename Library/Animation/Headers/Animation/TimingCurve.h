/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Geometry/Point.h>

namespace rl {
namespace animation {

class TimingCurve {
 public:
  using Data = uint8_t;
  enum class Type : Data {
    Linear,
    EaseIn,
    EaseOut,
    EaseInEaseOut,
  };

  static TimingCurve SystemTimingCurve(Type type);

  double x(double t) const;

 private:
  double _ax;
  double _bx;
  double _cx;
  double _ay;
  double _by;
  double _cy;

  TimingCurve(const geom::Point& c1, const geom::Point& c2);
};

}  // namespace animation
}  // namespace rl
