// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Vertices/PathVertices.h"

#include <algorithm>

#include <libtess2/tesselator.h>

namespace rl {
namespace compositor {

const int kVertexSize = 2;
const int kPolygonSize = 3;

static int ToTessElementType(PathVertices::ElementType type) {
  switch (type) {
    case PathVertices::ElementType::Polygons:
      return TESS_POLYGONS;
    case PathVertices::ElementType::ConnectedPolygons:
      return TESS_CONNECTED_POLYGONS;
    case PathVertices::ElementType::BoundaryContours:
      return TESS_BOUNDARY_CONTOURS;
  }
  return TESS_POLYGONS;
}

static int ToTessWindingRule(PathVertices::Winding winding) {
  switch (winding) {
    case PathVertices::Winding::Odd:
      return TESS_WINDING_ODD;
    case PathVertices::Winding::NonZero:
      return TESS_WINDING_NONZERO;
    case PathVertices::Winding::Positive:
      return TESS_WINDING_POSITIVE;
    case PathVertices::Winding::Negative:
      return TESS_WINDING_NEGATIVE;
    case PathVertices::Winding::AbsGeqTwo:
      return TESS_WINDING_ABS_GEQ_TWO;
  }
  return TESS_WINDING_ODD;
}

static inline geom::Point MinPoint(const geom::Point& a, const geom::Point& b) {
  return {std::min(a.x, b.x), std::min(a.y, b.y)};
}

static inline geom::Point MaxPoint(const geom::Point& a, const geom::Point& b) {
  return {std::max(a.x, b.x), std::max(a.y, b.y)};
}

static void AddPointAndTrackBounds(const geom::Point& point,
                                   std::vector<GLPoint>& contours,
                                   geom::Point& min,
                                   geom::Point& max) {
  contours.emplace_back(point);
  min = MinPoint(point, min);
  max = MaxPoint(point, max);
}

static std::pair<bool, geom::Size> PopulateContoursWithPath(
    TESStesselator* tessellator,
    const geom::Path& path) {
  if (tessellator == nullptr || path.componentCount() == 0) {
    return {false, {}};
  }

  std::vector<GLPoint> contours;

  geom::Point min, max;

  path.enumerateComponents(
      [&](size_t, const geom::LinearPathComponent& linear) {
        AddPointAndTrackBounds(linear.p1, contours, min, max);
        AddPointAndTrackBounds(linear.p2, contours, min, max);
      },
      [&](size_t, const geom::QuadraticPathComponent& quad) {
        for (const auto& point : quad.tessellate()) {
          AddPointAndTrackBounds(point, contours, min, max);
        }
      },
      [&](size_t, const geom::CubicPathComponent& cubic) {
        for (const auto& point : cubic.tessellate()) {
          AddPointAndTrackBounds(point, contours, min, max);
        }
      });

  if (contours.size() == 0) {
    return {false, {}};
  }

  double divisorX = max.x - min.x;
  double divisorY = max.y - min.y;

  for (auto& contourPoint : contours) {
    contourPoint.x = (contourPoint.x - min.x) / divisorX;
    contourPoint.y = (contourPoint.y - min.y) / divisorY;
  }

  tessAddContour(tessellator, kVertexSize, contours.data(), sizeof(GLPoint),
                 contours.size());

  return {true, {divisorX, divisorY}};
}

static void DestroyTessellator(TESStesselator* tessellator) {
  if (tessellator != nullptr) {
    tessDeleteTess(tessellator);
  }
}

PathVertices::PathVertices(const geom::Path& path,
                           Winding winding,
                           ElementType elementType)
    : Vertices(Vertices::Type::ElementArray) {
  if (path.componentCount() == 0) {
    return;
  }

  using Tessellator =
      std::unique_ptr<TESStesselator, decltype(&DestroyTessellator)>;

  Tessellator tessellator(tessNewTess(nullptr /* default allocators */),
                          DestroyTessellator);

  if (tessellator == nullptr) {
    return;
  }

  bool success = false;
  std::tie(success, _size) = PopulateContoursWithPath(tessellator.get(), path);

  if (!success) {
    return;
  }

  /*
   *  Perform tessellation.
   */
  auto result = tessTesselate(tessellator.get(),               // tessellator
                              ToTessWindingRule(winding),      // winding
                              ToTessElementType(elementType),  // element type
                              kPolygonSize,                    // polygon size
                              kVertexSize,                     // vertex size
                              nullptr                          // normal
                              );

  if (result != 1) {
    _size = {};
    return;
  }

  /*
   *  Copy vertices to ensure they are packed correctly for upload.
   */
  int vertexItemCount = tessGetVertexCount(tessellator.get()) * kVertexSize;
  auto vertices = tessGetVertices(tessellator.get());
  for (int i = 0; i < vertexItemCount; i += 2) {
    _vertices.emplace_back(vertices[i], vertices[i + 1]);
  }

  /*
   *  Copy elements to ensure they are packed correctly for upload.
   */
  int elementItemCount = tessGetElementCount(tessellator.get()) * kPolygonSize;
  auto elements = tessGetElements(tessellator.get());
  for (int i = 0; i < elementItemCount; i++) {
    _elements.emplace_back(elements[i]);
  }
}

PathVertices::~PathVertices() = default;

const geom::Size& PathVertices::size() const {
  return _size;
}

bool PathVertices::hasVerticesAndElements() const {
  return _vertices.size() > 0 && _elements.size() > 0;
}

bool PathVertices::uploadVertexData() {
  glBufferData(GL_ARRAY_BUFFER,
               _vertices.size() * sizeof(decltype(_vertices)::value_type),
               _vertices.data(), GL_STATIC_DRAW);

  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               _elements.size() * sizeof(decltype(_elements)::value_type),
               _elements.data(), GL_STATIC_DRAW);

  return true;
}

bool PathVertices::doDraw(size_t index) {
  glVertexAttribPointer(index, kVertexSize, GL_FLOAT, GL_FALSE, 0, nullptr);

  glEnableVertexAttribArray(index);

  glDrawElements(GL_TRIANGLES, _elements.size(), GL_UNSIGNED_SHORT, nullptr);

  glDisableVertexAttribArray(index);

  return true;
}

}  // namespace compositor
}  // namespace rl
