/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <libtess2/tesselator.h>
#include <algorithm>
#include "Console.h"
#include "Vertices/PathVertices.h"

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

static std::pair<bool, geom::Size> PopulateContoursWithPath(
    TESStesselator* tessellator,
    const geom::Path& path) {
  /*
   *  Make sure the path has valid components.
   */
  if (tessellator == nullptr || path.componentCount() == 0) {
    return {false, {}};
  }

  /*
   *  Ensure the bounding box of the path can be calculated so that vertex
   *  calculations and texture mapping is correctly setup at scale.
   */
  auto boundingBox = path.boundingBox();
  if (boundingBox.size.width <= 0.0 || boundingBox.size.height <= 0.0) {
    return {false, {}};
  }

  /*
   *  Smoothen path.
   */
  geom::SmoothingApproximation defaultApproximation;
  auto tessellatedPoints = path.smoothen(defaultApproximation);

  if (tessellatedPoints.size() == 0) {
    return {false, {}};
  }

  std::vector<gl::GLPoint> contours;
  contours.reserve(tessellatedPoints.size());
  for (const auto& point : tessellatedPoints) {
    contours.emplace_back(point.x / boundingBox.size.width,
                          point.y / boundingBox.size.height);
  }

  tessAddContour(tessellator, kVertexSize, contours.data(), sizeof(gl::GLPoint),
                 contours.size());

  return {true, boundingBox.size};
}

static void DestroyTessellator(TESStesselator* tessellator) {
  if (tessellator != nullptr) {
    tessDeleteTess(tessellator);
  }
}

PathVertices::PathVertices(const geom::Path& path,
                           Winding winding,
                           ElementType elementType)
    : Vertices(Vertices::Type::ElementArray), _type(elementType) {
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

static GLenum GLModeForElementType(PathVertices::ElementType type) {
  const auto showPathMesh = RL_CONSOLE_GET_VALUE_ONCE("Show Path Mesh", false);

  if (showPathMesh) {
    return GL_LINES;
  }

  switch (type) {
    case PathVertices::ElementType::Polygons:
      return GL_TRIANGLES;
    case PathVertices::ElementType::BoundaryContours:
      return GL_LINES;
    case PathVertices::ElementType::ConnectedPolygons:
      /*
       *  Unimplemented.
       */
      RL_ASSERT(false);
      break;
  }

  return GL_LINES;
}

bool PathVertices::doDraw(size_t index) const {
  glVertexAttribPointer(index, kVertexSize, GL_FLOAT, GL_FALSE, 0, nullptr);

  glEnableVertexAttribArray(index);

  glDrawElements(GLModeForElementType(_type), _elements.size(),
                 GL_UNSIGNED_SHORT, nullptr);

  glDisableVertexAttribArray(index);

  return true;
}

}  // namespace compositor
}  // namespace rl
