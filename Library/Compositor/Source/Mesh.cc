// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Mesh.h"

#include <libtess2/tesselator.h>

namespace rl {
namespace compositor {

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

Mesh::Mesh(const geom::Path& path, Winding winding, ElementType elementType) {
  if (path.componentCount() == 0) {
    return;
  }

  auto tessellator = tessNewTess(nullptr /* default allocators */);

  if (tessellator == nullptr) {
    return;
  }

  const int kPolygonSize = 3;
  const int kVertexSize = 2;

  /*
   *  Perform tessellation.
   */
  auto result = tessTesselate(tessellator,                     // tessellator
                              ToTessWindingRule(winding),      // winding
                              ToTessElementType(elementType),  // element type
                              kPolygonSize,                    // polygon size
                              kVertexSize,                     // vertex size
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
   *  Copy vertices out.
   */
  auto vertices = tessGetVertices(tessellator);
  for (int i = 0; i < vertexCount; i++) {
    _vertices.emplace_back(vertices[i * kVertexSize],     // x
                           vertices[i * kVertexSize + 1]  // y
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
