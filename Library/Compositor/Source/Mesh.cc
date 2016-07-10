// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Mesh.h"

#include <libtess2/tesselator.h>

namespace rl {
namespace compositor {

const int kTessellationVertexSize = 2;

static int ToTessElementType(Mesh::ElementType type) {
  switch (type) {
    case Mesh::ElementType::Polygons:
      return TESS_POLYGONS;
    case Mesh::ElementType::ConnectedPolygons:
      return TESS_CONNECTED_POLYGONS;
    case Mesh::ElementType::BoundaryContours:
      return TESS_BOUNDARY_CONTOURS;
  }
  return TESS_POLYGONS;
}

static int ToTessWindingRule(Mesh::Winding winding) {
  switch (winding) {
    case Mesh::Winding::Odd:
      return TESS_WINDING_ODD;
    case Mesh::Winding::NonZero:
      return TESS_WINDING_NONZERO;
    case Mesh::Winding::Positive:
      return TESS_WINDING_POSITIVE;
    case Mesh::Winding::Negative:
      return TESS_WINDING_NEGATIVE;
    case Mesh::Winding::AbsGeqTwo:
      return TESS_WINDING_ABS_GEQ_TWO;
  }
  return TESS_WINDING_ODD;
}

static bool PopulateContoursWithPath(TESStesselator* tess,
                                     const geom::Path& path) {
  if (tess == nullptr || path.componentCount() == 0) {
    return false;
  }

  const size_t kVerticesPerContour = 12;

  std::vector<GLPoint> contours;

  /*
   *  TODO: While enumerating path components like this, points at the end of
   *        each component will be repeated in the countours vector. This can
   *        be optimized by keeping track of the last contour point added and
   *        only adding the next point if the interpolation leads to a
   *        significant change.
   */

  path.enumerateComponents(
      [&](size_t, const geom::LinearPathComponent& linear) {
        contours.emplace_back(linear.p1);
        contours.emplace_back(linear.p2);
      },
      [&](size_t, const geom::QuadraticPathComponent& quad) {
        for (size_t i = 0; i < kVerticesPerContour; i++) {
          contours.emplace_back(
              quad.solve(static_cast<double>(i) / kVerticesPerContour));
        }
      },
      [&](size_t, const geom::CubicPathComponent& cubic) {
        for (size_t i = 0; i < kVerticesPerContour; i++) {
          contours.emplace_back(
              cubic.solve(static_cast<double>(i) / kVerticesPerContour));
        }
      });

  if (contours.size() == 0) {
    return false;
  }

  tessAddContour(tess, kTessellationVertexSize, contours.data(),
                 sizeof(GLPoint), contours.size());

  return true;
}

Mesh::Mesh(const geom::Path& path, Winding winding, ElementType elementType) {
  if (path.componentCount() == 0) {
    return;
  }

  auto tessellator = tessNewTess(nullptr /* default allocators */);

  if (tessellator == nullptr) {
    return;
  }

  if (!PopulateContoursWithPath(tessellator, path)) {
    tessDeleteTess(tessellator);
    return;
  }

  const int kPolygonSize = 3;

  /*
   *  Perform tessellation.
   */
  auto result = tessTesselate(tessellator,                     // tessellator
                              ToTessWindingRule(winding),      // winding
                              ToTessElementType(elementType),  // element type
                              kPolygonSize,                    // polygon size
                              kTessellationVertexSize,         // vertex size
                              nullptr                          // normal
                              );

  if (result != 1) {
    tessDeleteTess(tessellator);
    return;
  }

  /*
   *  Get vertex and element counts out of libtess2.
   */
  const int vertexCount = tessGetVertexCount(tessellator);
  const int elementCount = tessGetElementCount(tessellator) * kPolygonSize;

  /*
   *  Reserve space in our vectors to prevent too many reallocations.
   */
  _vertices.reserve(vertexCount);
  _elements.reserve(elementCount);

  /*
   *  TODO: These copies are unnecessary. Just keep a reference to the
   *        tessellator and use the vertices and indices directly.
   */

  /*
   *  Copy vertices out.
   */
  auto vertices = tessGetVertices(tessellator);
  for (int i = 0; i < vertexCount; i++) {
    _vertices.emplace_back(vertices[i * kTessellationVertexSize],     // x
                           vertices[i * kTessellationVertexSize + 1]  // y
                           );
  }

  /*
   *  Copy elements out.
   */
  auto elements = tessGetElements(tessellator);
  for (int i = 0; i < elementCount; i++) {
    _elements.emplace_back(elements[i]);
  }

  tessDeleteTess(tessellator);
}

Mesh::~Mesh() = default;

}  // namespace compositor
}  // namespace rl
