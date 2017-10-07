/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
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

  ~Shape() = default;

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

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(EllipseShape);
};

class FillShape : public Shape {
 public:
  FillShape() = default;

  ~FillShape() = default;

  Type type() const override { return Type::Fill; }

 private:
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

 private:
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
  StarShape() = default;

  ~StarShape() = default;

  Type type() const override { return Type::Star; }

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(StarShape);
};

class VertexShape : public Shape {
 public:
  VertexShape() = default;

  ~VertexShape() = default;

  Type type() const override { return Type::Vertex; }

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(VertexShape);
};

class StrokeShape : public Shape {
 public:
  StrokeShape() = default;

  ~StrokeShape() = default;

  Type type() const override { return Type::Stroke; }

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(StrokeShape);
};

class TransformShape : public Shape {
 public:
  TransformShape() = default;

  ~TransformShape() = default;

  Type type() const override { return Type::Transform; }

 private:
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
