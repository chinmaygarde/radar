/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <string>

namespace rl {
namespace bodymovin {

class Shape {
 public:
  Shape() = default;

  ~Shape() = default;

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

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(EllipseShape);
};

class FillShape : public Shape {
 public:
  FillShape() = default;

  ~FillShape() = default;

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(FillShape);
};

class GradientFillShape : public Shape {
 public:
  GradientFillShape() = default;

  ~GradientFillShape() = default;

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(GradientFillShape);
};

class GroupShape : public Shape {
 public:
  GroupShape() = default;

  ~GroupShape() = default;

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(GroupShape);
};

class GradientStrokeShape : public Shape {
 public:
  GradientStrokeShape() = default;

  ~GradientStrokeShape() = default;

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(GradientStrokeShape);
};

class MergeShape : public Shape {
 public:
  MergeShape() = default;

  ~MergeShape() = default;

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(MergeShape);
};

class RectShape : public Shape {
 public:
  RectShape() = default;

  ~RectShape() = default;

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(RectShape);
};

class RoundShape : public Shape {
 public:
  RoundShape() = default;

  ~RoundShape() = default;

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(RoundShape);
};

class StarShape : public Shape {
 public:
  StarShape() = default;

  ~StarShape() = default;

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(StarShape);
};

class VertexShape : public Shape {
 public:
  VertexShape() = default;

  ~VertexShape() = default;

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(VertexShape);
};

class StrokeShape : public Shape {
 public:
  StrokeShape() = default;

  ~StrokeShape() = default;

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(StrokeShape);
};

class TrimShape : public Shape {
 public:
  TrimShape() = default;

  ~TrimShape() = default;

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(TrimShape);
};

}  // namespace bodymovin
}  // namespace rl
