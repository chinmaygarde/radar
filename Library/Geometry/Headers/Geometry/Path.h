// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_GEOMETRY_PATH_H_
#define RADAR_GEOMETRY_PATH_H_

#include <Core/Macros.h>
#include <Geometry/PathComponent.h>

#include <vector>
#include <functional>

namespace rl {
namespace geom {

class Path {
 public:
  enum class ComponentType : uint8_t {
    Linear,
    Quadratic,
    Cubic,
  };

  Path();

  ~Path();

  size_t componentCount() const;

  Path& addLinearComponent(Point p1, Point p2);

  Path& addQuadraticComponent(Point p1, Point cp, Point p2);

  Path& addCubicComponent(Point p1, Point cp1, Point cp2, Point p2);

  template <class T>
  using Applier = std::function<void(size_t index, const T& component)>;
  void enumerateComponents(Applier<LinearPathComponent> linearApplier,
                           Applier<QuadraticPathComponent> quadApplier,
                           Applier<CubicPathComponent> cubicApplier) const;

 private:
  using ComponentIndexPair = std::pair<ComponentType, size_t>;

  std::vector<ComponentIndexPair> _components;
  std::vector<LinearPathComponent> _linears;
  std::vector<QuadraticPathComponent> _quads;
  std::vector<CubicPathComponent> _cubics;

  RL_DISALLOW_COPY_AND_ASSIGN(Path);
};

}  // namespace geom
}  // namespace rl

#endif  // RADAR_GEOMETRY_PATH_H_
