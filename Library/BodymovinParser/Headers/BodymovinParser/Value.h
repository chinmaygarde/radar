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
  ValueBase() = default;

  virtual ~ValueBase() = default;

  const std::string& expression() const { return _expression; }

  uint64_t index() const { return _index; }

  bool isAnimated() const { return _isAnimated; }

  void setExpression(std::string expression) {
    _expression = std::move(expression);
  }

  void setIndex(uint64_t index) { _index = index; }

  void setIsAnimated(bool isAnimated) { _isAnimated = isAnimated; }

 private:
  std::string _expression;
  uint64_t _index = 0;
  bool _isAnimated = false;

  RL_DISALLOW_COPY_AND_ASSIGN(ValueBase);
};

class Value : public ValueBase {
 public:
  Value() = default;

  ~Value() override = default;

  double value() const { return _value; }

  void setValue(double value) { _value = value; }

 private:
  double _value = 0.0;

  RL_DISALLOW_COPY_AND_ASSIGN(Value);
};

class KeyframedValue : public ValueBase {
 public:
  KeyframedValue() = default;

  ~KeyframedValue() override = default;

  double keyframeEnd() const { return _keyframeEnd; }

  double keyframeStart() const { return _keyframeStart; }

  double startTime() const { return _startTime; }

  const geom::Point& bezierCurveIn() const { return _bezierCurveIn; }

  const geom::Point& bezierCurveOut() const { return _bezierCurveOut; }

  const std::string& bezierCurveName() const { return _bezierCurveName; }

  void setKeyframeEnd(double keyframeEnd) { _keyframeEnd = keyframeEnd; }

  void setKeyframeStart(double keyframeStart) {
    _keyframeStart = keyframeStart;
  }

  void setStartTime(double startTime) { _startTime = startTime; }

  void setBezierCurveIn(const geom::Point& bezierCurveIn) {
    _bezierCurveIn = bezierCurveIn;
  }

  void setBezierCurveOut(const geom::Point& bezierCurveOut) {
    _bezierCurveOut = bezierCurveOut;
  }

  void setBezierCurveName(std::string bezierCurveName) {
    _bezierCurveName = std::move(bezierCurveName);
  }

 private:
  double _keyframeEnd = 0.0;
  double _keyframeStart = 0.0;
  double _startTime = 0.0;
  geom::Point _bezierCurveIn;
  geom::Point _bezierCurveOut;
  std::string _bezierCurveName;

  RL_DISALLOW_COPY_AND_ASSIGN(KeyframedValue);
};

}  // namespace bodymovin
}  // namespace rl
