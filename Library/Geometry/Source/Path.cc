// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Geometry/Path.h>

namespace rl {
namespace geom {

Path::Path() = default;

Path::~Path() = default;

size_t Path::componentCount() const {
  return _components.size();
}

Path& Path::addLinearComponent(Point p1, Point p2) {
  _linears.emplace_back(p1, p2);
  _components.emplace_back(ComponentType::Linear, _linears.size() - 1);
  return *this;
}

Path& Path::addQuadraticComponent(Point p1, Point cp, Point p2) {
  _quads.emplace_back(p1, cp, p2);
  _components.emplace_back(ComponentType::Quadratic, _quads.size() - 1);
  return *this;
}

Path& Path::addCubicComponent(Point p1, Point cp1, Point cp2, Point p2) {
  _cubics.emplace_back(p1, cp1, cp2, p2);
  _components.emplace_back(ComponentType::Cubic, _cubics.size() - 1);
  return *this;
}

void Path::enumerateComponents(Applier<LinearPathComponent> linearApplier,
                               Applier<QuadraticPathComponent> quadApplier,
                               Applier<CubicPathComponent> cubicApplier) const {
  size_t currentIndex = 0;
  for (const auto& component : _components) {
    switch (component.first) {
      case ComponentType::Linear:
        if (linearApplier) {
          linearApplier(currentIndex, _linears[component.second]);
        }
        break;
      case ComponentType::Quadratic:
        if (quadApplier) {
          quadApplier(currentIndex, _quads[component.second]);
        }
        break;
      case ComponentType::Cubic:
        if (cubicApplier) {
          cubicApplier(currentIndex, _cubics[component.second]);
        }
        break;
    }
    currentIndex++;
  }
}

}  // namespace geom
}  // namespace rl
