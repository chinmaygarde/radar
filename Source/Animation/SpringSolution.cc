// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Animation/SpringSolution.h>

namespace rl {
namespace animation {

class CriticalSolution : public SpringSolution {
 public:
  CriticalSolution(double r, double c1, double c2) : _r(r), _c1(c1), _c2(c2) {}

  static std::unique_ptr<CriticalSolution> Create(SpringDescription desc,
                                                  double distance,
                                                  double velocity) {
    const double r = -desc.damping / (2.0 * desc.mass);
    const double c1 = distance;
    const double c2 = velocity / (r * distance);
    return core::make_unique<CriticalSolution>(r, c1, c2);
  }

  double x(const core::ClockDuration& time) override {
    return (_c1 + _c2 * time.count()) * pow(M_E, _r * time.count());
  }

  double dx(const core::ClockDuration& time) override {
    const double power = pow(M_E, _r * time.count());
    return _r * (_c1 + _c2 * time.count()) * power + _c2 * power;
  }

  bool isDone(const core::ClockDuration& time) override { return false; }

 private:
  const double _r;
  const double _c1;
  const double _c2;

  RL_DISALLOW_COPY_AND_ASSIGN(CriticalSolution);
};

class OverdampedSolution : public SpringSolution {
 public:
  OverdampedSolution(double r1, double r2, double c1, double c2)
      : _r1(r1), _r2(r2), _c1(c1), _c2(c2) {}

  static std::unique_ptr<OverdampedSolution> Create(SpringDescription desc,
                                                    double distance,
                                                    double velocity) {
    const double cmk =
        desc.damping * desc.damping - 4 * desc.mass * desc.springConstant;

    const double r1 = (-desc.damping - sqrt(cmk)) / (2.0 * desc.mass);
    const double r2 = (-desc.damping + sqrt(cmk)) / (2.0 * desc.mass);
    const double c2 = (velocity - r1 * distance) / (r2 - r1);
    const double c1 = distance - c2;

    return core::make_unique<OverdampedSolution>(r1, r2, c1, c2);
  }

  double x(const core::ClockDuration& time) override {
    return (_c1 * pow(M_E, _r1 * time.count()) +
            _c2 * pow(M_E, _r2 * time.count()));
  }

  double dx(const core::ClockDuration& time) override {
    return (_c1 * _r1 * pow(M_E, _r1 * time.count()) +
            _c2 * _r2 * pow(M_E, _r2 * time.count()));
  }

  bool isDone(const core::ClockDuration& time) override { return false; }

 private:
  const double _r1;
  const double _r2;
  const double _c1;
  const double _c2;

  RL_DISALLOW_COPY_AND_ASSIGN(OverdampedSolution);
};

class UnderdampedSolution : public SpringSolution {
 public:
  UnderdampedSolution(double w, double r, double c1, double c2)
      : _w(w), _r(r), _c1(c1), _c2(c2) {}

  static std::unique_ptr<UnderdampedSolution> Create(SpringDescription desc,
                                                     double distance,
                                                     double velocity) {
    const double w = sqrt(4.0 * desc.mass * desc.springConstant -
                          desc.damping * desc.damping) /
                     (2.0 * desc.mass);

    const double r = -(desc.damping / 2.0 * desc.mass);
    const double c1 = distance;
    const double c2 = (velocity - r * distance) / w;

    return core::make_unique<UnderdampedSolution>(w, r, c1, c2);
  }

  double x(const core::ClockDuration& time) override {
    return pow(M_E, _r * time.count()) *
           (_c1 * cos(_w * time.count()) + _c2 * sin(_w * time.count()));
  }

  double dx(const core::ClockDuration& time) override {
    double power = pow(M_E, _r * time.count());
    double cosine = cos(_w * time.count());
    double sine = sin(_w * time.count());

    return power * (_c2 * _w * cosine - _c1 * _w * sine) +
           _r * power * (_c2 * sine + _c1 * cosine);
  }

  bool isDone(const core::ClockDuration& time) override { return false; }

 private:
  const double _w;
  const double _r;
  const double _c1;
  const double _c2;

  RL_DISALLOW_COPY_AND_ASSIGN(UnderdampedSolution);
};

std::unique_ptr<SpringSolution> SpringSolution::Create(SpringDescription desc,
                                                       double initialPosition,
                                                       double initialVelocity) {
  double cmk =
      desc.damping * desc.damping - 4 * desc.mass * desc.springConstant;

  if (cmk == 0.0) {
    return CriticalSolution::Create(desc, initialPosition, initialVelocity);
  } else if (cmk > 0.0) {
    return OverdampedSolution::Create(desc, initialPosition, initialVelocity);
  } else {
    return UnderdampedSolution::Create(desc, initialPosition, initialVelocity);
  }

  RL_ASSERT_MSG(false, "Could not create a spring solution");
  return nullptr;
}

}  // namespace animation
}  // namespace rl
