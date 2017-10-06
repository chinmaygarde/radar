/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Geometry/Point.h>
#include <string>

namespace rl {
namespace bodymovin {

class ValueBase {
 public:
  ValueBase();

  virtual ~ValueBase();

  double value() const;

  const std::string& expression() const;

  const std::string& index() const;

  bool isAnimated() const;

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(ValueBase);
};

class Value : public ValueBase {
 public:
  double value() const;

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(Value);
};

class KeyframedValue : public ValueBase {
 public:
  KeyframedValue();

  ~KeyframedValue();

  double keyframeEnd() const;

  double keyframeStart() const;

  double startTime() const;

  const geom::Point& bezierCurveIn() const;

  const geom::Point& bezierCurveOut() const;

  const std::string& bezierCurveName() const;

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(KeyframedValue);
};

}  // namespace bodymovin
}  // namespace rl
