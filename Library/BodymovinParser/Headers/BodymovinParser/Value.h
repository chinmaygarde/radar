/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Geometry/Point.h>
#include <string>
#include <vector>

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

template <class T>
class ValueT : public ValueBase {
 public:
  using Type = T;

  ValueT() = default;

  ~ValueT() override = default;

  Type value() const { return _value; }

  void setValue(Type value) { _value = std::move(value); }

 private:
  Type _value = 0.0;

  RL_DISALLOW_COPY_AND_ASSIGN(ValueT);
};

using Value = ValueT<double>;

using MultidimensionalValue = ValueT<std::vector<double>>;

template <class T>
class KeyframedValueT : public ValueBase {
 public:
  using Type = T;

  KeyframedValueT() = default;

  ~KeyframedValueT() override = default;

  Type keyframeEnd() const { return _keyframeEnd; }

  Type keyframeStart() const { return _keyframeStart; }

  Type startTime() const { return _startTime; }

  const Type& bezierCurveInX() const { return _bezierCurveInX; }

  const Type& bezierCurveInY() const { return _bezierCurveInY; }

  const Type& bezierCurveOutX() const { return _bezierCurveOutX; }

  const Type& bezierCurveOutY() const { return _bezierCurveOutY; }

  const std::string& bezierCurveName() const { return _bezierCurveName; }

  void setKeyframeEnd(Type keyframeEnd) {
    _keyframeEnd = std::move(keyframeEnd);
  }

  void setKeyframeStart(Type keyframeStart) {
    _keyframeStart = std::move(keyframeStart);
  }

  void setStartTime(Type startTime) { _startTime = std::move(startTime); }

  void setBezierCurveInX(Type bezierCurveInX) {
    _bezierCurveInX = std::move(bezierCurveInX);
  }

  void setBezierCurveInY(Type bezierCurveInY) {
    _bezierCurveInY = std::move(bezierCurveInY);
  }

  void setBezierCurveOutX(Type bezierCurveOutX) {
    _bezierCurveOutX = std::move(bezierCurveOutX);
  }

  void setBezierCurveOutY(Type bezierCurveOutY) {
    _bezierCurveOutY = std::move(bezierCurveOutY);
  }

  void setBezierCurveName(std::string bezierCurveName) {
    _bezierCurveName = std::move(bezierCurveName);
  }

 private:
  Type _keyframeEnd = 0;
  Type _keyframeStart = 0;
  Type _startTime = 0;
  Type _bezierCurveInX = 0;
  Type _bezierCurveInY = 0;
  Type _bezierCurveOutX = 0;
  Type _bezierCurveOutY = 0;
  std::string _bezierCurveName;

  RL_DISALLOW_COPY_AND_ASSIGN(KeyframedValueT);
};

using KeyframedValue = KeyframedValueT<double>;

using MultidimensionalKeyframedValue = KeyframedValueT<std::vector<double>>;

}  // namespace bodymovin
}  // namespace rl
