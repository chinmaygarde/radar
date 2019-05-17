/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <BodymovinParser/Types.h>
#include <BodymovinParser/Value.h>
#include <Core/Macros.h>
#include <memory>
#include <string>
#include <vector>

namespace rl {
namespace bodymovin {

class Shape {
 public:
  enum class Type {
    Ellipse,
    Fill,
    GradientFill,
    Group,
    GradientStroke,
    Merge,
    Rect,
    Round,
    Vertex,
    Star,
    Stroke,
    Transform,
    Trim,
  };

  Shape() = default;

  virtual ~Shape() = default;

  virtual Type type() const = 0;

  const std::string& matchName() const { return _matchName; }

  const std::string& name() const { return _name; }

  double direction() const { return _direction; }

  void setMatchName(std::string matchName) {
    _matchName = std::move(matchName);
  }

  void setName(std::string name) { _name = std::move(name); }

  void setDirection(double direction) { _direction = direction; }

 private:
  std::string _matchName;
  std::string _name;
  double _direction = 0.0;

  RL_DISALLOW_COPY_AND_ASSIGN(Shape);
};

class EllipseShape : public Shape {
 public:
  EllipseShape() = default;

  ~EllipseShape() = default;

  Type type() const override { return Type::Ellipse; }

  const std::unique_ptr<ValueBase>& position() const { return _position; }

  const std::unique_ptr<ValueBase>& size() const { return _size; }

  void setPosition(std::unique_ptr<ValueBase> position) {
    _position = std::move(position);
  }

  void setSize(std::unique_ptr<ValueBase> size) { _size = std::move(size); }

 private:
  std::unique_ptr<ValueBase> _position;
  std::unique_ptr<ValueBase> _size;

  RL_DISALLOW_COPY_AND_ASSIGN(EllipseShape);
};

class FillShape : public Shape {
 public:
  FillShape() = default;

  ~FillShape() = default;

  Type type() const override { return Type::Fill; }

  const std::unique_ptr<ValueBase>& opacity() const { return _opacity; }

  const std::unique_ptr<ValueBase>& color() const { return _color; }

  void setOpacity(std::unique_ptr<ValueBase> opacity) {
    _opacity = std::move(opacity);
  }

  void setColor(std::unique_ptr<ValueBase> color) { _color = std::move(color); }

 private:
  std::unique_ptr<ValueBase> _opacity;
  std::unique_ptr<ValueBase> _color;

  RL_DISALLOW_COPY_AND_ASSIGN(FillShape);
};

class GradientFillShape : public Shape {
 public:
  GradientFillShape() = default;

  ~GradientFillShape() = default;

  Type type() const override { return Type::GradientFill; }

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(GradientFillShape);
};

class GroupShape : public Shape {
 public:
  GroupShape() = default;

  ~GroupShape() = default;

  Type type() const override { return Type::Group; }

  uint64_t propertiesCount() const { return _propertiesCount; }

  void setPropertiesCount(uint64_t count) { _propertiesCount = count; }

  const std::vector<std::unique_ptr<Shape>>& shapeItems() const {
    return _shapeItems;
  }

  void setShapeItems(std::vector<std::unique_ptr<Shape>> shapeItems) {
    _shapeItems = std::move(shapeItems);
  }

 private:
  uint64_t _propertiesCount = 0;
  std::vector<std::unique_ptr<Shape>> _shapeItems;

  RL_DISALLOW_COPY_AND_ASSIGN(GroupShape);
};

class GradientStrokeShape : public Shape {
 public:
  GradientStrokeShape() = default;

  ~GradientStrokeShape() = default;

  Type type() const override { return Type::GradientStroke; }

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(GradientStrokeShape);
};

class MergeShape : public Shape {
 public:
  MergeShape() = default;

  ~MergeShape() = default;

  Type type() const override { return Type::Merge; }

  double mergeMode() const { return _mergeMode; }

  void setMergeMode(double mergeMode) { _mergeMode = mergeMode; }

 private:
  double _mergeMode = 0.0;
  RL_DISALLOW_COPY_AND_ASSIGN(MergeShape);
};

class RectShape : public Shape {
 public:
  RectShape() = default;

  ~RectShape() = default;

  Type type() const override { return Type::Rect; }

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(RectShape);
};

class RoundShape : public Shape {
 public:
  RoundShape() = default;

  ~RoundShape() = default;

  Type type() const override { return Type::Round; }

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(RoundShape);
};

class StarShape : public Shape {
 public:
  enum class StarType {
    Star,
    Polygon,
  };

  StarShape() = default;

  ~StarShape() = default;

  Type type() const override { return Type::Star; }

  StarType starType() const { return _starType; }

  const std::unique_ptr<ValueBase>& position() const { return _position; }

  const std::unique_ptr<ValueBase>& innerRadius() const { return _innerRadius; }

  const std::unique_ptr<ValueBase>& outerRoundness() const {
    return _outerRoundness;
  }

  const std::unique_ptr<ValueBase>& rotation() const { return _rotation; }

  const std::unique_ptr<ValueBase>& points() const { return _points; }

  void setStarType(StarType type) { _starType = type; }

  void setPosition(std::unique_ptr<ValueBase> position) {
    _position = std::move(position);
  }

  void setInnerRadius(std::unique_ptr<ValueBase> innerRadius) {
    _innerRadius = std::move(innerRadius);
  }

  void setOuterRoundness(std::unique_ptr<ValueBase> outerRoundness) {
    _outerRoundness = std::move(outerRoundness);
  }

  void setRotation(std::unique_ptr<ValueBase> rotation) {
    _rotation = std::move(rotation);
  }

  void setPoints(std::unique_ptr<ValueBase> points) {
    _points = std::move(points);
  }

 private:
  StarType _starType = StarType::Star;
  std::unique_ptr<ValueBase> _position;
  std::unique_ptr<ValueBase> _innerRadius;
  std::unique_ptr<ValueBase> _outerRoundness;
  std::unique_ptr<ValueBase> _rotation;
  std::unique_ptr<ValueBase> _points;

  RL_DISALLOW_COPY_AND_ASSIGN(StarShape);
};

class VertexShape : public Shape {
 public:
  VertexShape() = default;

  ~VertexShape() = default;

  Type type() const override { return Type::Vertex; }

  void setShapeVertices(std::unique_ptr<ValueBase> vertices) {
    _shapeVertices = std::move(vertices);
  }

  const std::unique_ptr<ValueBase>& shapeVertices() const {
    return _shapeVertices;
  }

 private:
  std::unique_ptr<ValueBase> _shapeVertices;

  RL_DISALLOW_COPY_AND_ASSIGN(VertexShape);
};

class StrokeShape : public Shape {
 public:
  StrokeShape() = default;

  ~StrokeShape() = default;

  LineCap lineCap() const { return _lineCap; }

  LineJoin lineJoin() const { return _lineJoin; }

  double miterLimit() const { return _miterLimit; }

  const std::unique_ptr<ValueBase>& opacity() const { return _opacity; }

  const std::unique_ptr<ValueBase>& width() const { return _width; }

  const std::unique_ptr<ValueBase>& color() const { return _color; }

  Type type() const override { return Type::Stroke; }

  void setLineCap(LineCap lineCap) { _lineCap = lineCap; }

  void setLineJoin(LineJoin lineJoin) { _lineJoin = lineJoin; }

  void setMiterLimit(double miterLimit) { _miterLimit = miterLimit; }

  void setOpacity(std::unique_ptr<ValueBase> opacity) {
    _opacity = std::move(opacity);
  }

  void setWidth(std::unique_ptr<ValueBase> width) { _width = std::move(width); }

  void setColor(std::unique_ptr<ValueBase> color) { _color = std::move(color); }

 private:
  LineCap _lineCap = LineCap::Butt;
  LineJoin _lineJoin = LineJoin::Miter;
  double _miterLimit = 0;
  std::unique_ptr<ValueBase> _opacity;
  std::unique_ptr<ValueBase> _width;
  std::unique_ptr<ValueBase> _color;

  RL_DISALLOW_COPY_AND_ASSIGN(StrokeShape);
};

class TransformShape : public Shape {
 public:
  TransformShape() = default;

  ~TransformShape() = default;

  Type type() const override { return Type::Transform; }

  const std::unique_ptr<ValueBase>& anchorPoint() const { return _anchorPoint; }

  const std::unique_ptr<ValueBase>& position() const { return _position; }

  const std::unique_ptr<ValueBase>& scale() const { return _scale; }

  const std::unique_ptr<ValueBase>& rotation() const { return _rotation; }

  const std::unique_ptr<ValueBase>& opacity() const { return _opacity; }

  const std::unique_ptr<ValueBase>& skew() const { return _skew; }

  const std::unique_ptr<ValueBase>& skewAxis() const { return _skewAxis; }

  void setAnchorPoint(std::unique_ptr<ValueBase> anchorPoint) {
    _anchorPoint = std::move(anchorPoint);
  }

  void setPosition(std::unique_ptr<ValueBase> position) {
    _position = std::move(position);
  }

  void setScale(std::unique_ptr<ValueBase> scale) { _scale = std::move(scale); }

  void setRotation(std::unique_ptr<ValueBase> rotation) {
    _rotation = std::move(rotation);
  }

  void setOpacity(std::unique_ptr<ValueBase> opacity) {
    _opacity = std::move(opacity);
  }

  void setSkew(std::unique_ptr<ValueBase> skew) { _skew = std::move(skew); }

  void setSkewAxis(std::unique_ptr<ValueBase> skewAxis) {
    _skewAxis = std::move(skewAxis);
  }

 private:
  std::unique_ptr<ValueBase> _anchorPoint;
  std::unique_ptr<ValueBase> _position;
  std::unique_ptr<ValueBase> _scale;
  std::unique_ptr<ValueBase> _rotation;
  std::unique_ptr<ValueBase> _opacity;
  std::unique_ptr<ValueBase> _skew;
  std::unique_ptr<ValueBase> _skewAxis;

  RL_DISALLOW_COPY_AND_ASSIGN(TransformShape);
};

class TrimShape : public Shape {
 public:
  TrimShape() = default;

  ~TrimShape() = default;

  Type type() const override { return Type::Trim; }

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(TrimShape);
};

}  // namespace bodymovin
}  // namespace rl
