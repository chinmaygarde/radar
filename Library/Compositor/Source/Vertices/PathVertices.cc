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

static int ToTessElementType(PathVertices::Type type) {
  switch (type) {
    case PathVertices::Type::Fill:
      return TESS_POLYGONS;
    case PathVertices::Type::Stroke:
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

static std::pair<bool, geom::Size> PopulateFillWithPath(
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
  auto tessellatedPoints = path.smoothPoints(defaultApproximation);

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

static Vertices::Type VerticesTypeForPathType(PathVertices::Type type) {
  switch (type) {
    case PathVertices::Type::Fill:
      return Vertices::Type::ElementArray;
    case PathVertices::Type::Stroke:
      return Vertices::Type::Array;
  }
  return Vertices::Type::Array;
}

PathVertices::PathVertices(const geom::Path& path, Type type, Winding winding)
    : Vertices(VerticesTypeForPathType(type)), _type(type), _winding(winding) {
  if (path.componentCount() == 0) {
    return;
  }

  switch (type) {
    case Type::Fill:
      setupFill(path);
      break;
    case Type::Stroke:
      setupStroke(path);
      break;
  }
}

void PathVertices::setupFill(const geom::Path& path) {
  using Tessellator =
      std::unique_ptr<TESStesselator, decltype(&DestroyTessellator)>;

  Tessellator tessellator(tessNewTess(nullptr /* default allocators */),
                          DestroyTessellator);

  if (tessellator == nullptr) {
    return;
  }

  bool success = false;
  std::tie(success, _size) = PopulateFillWithPath(tessellator.get(), path);

  if (!success) {
    return;
  }

  /*
   *  Perform tessellation.
   */
  auto result = tessTesselate(tessellator.get(),            // tessellator
                              ToTessWindingRule(_winding),  // winding
                              ToTessElementType(_type),     // element type
                              kPolygonSize,                 // polygon size
                              kVertexSize,                  // vertex size
                              nullptr                       // normal
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

void PathVertices::setupStroke(const geom::Path& path) {
  _size = path.boundingBox().size;
  geom::SmoothingApproximation defaultApproximation;
  auto points = path.smoothPoints(defaultApproximation);
  for (const auto& point : points) {
    _vertices.emplace_back(point.x / _size.width, point.y / _size.height);
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

  if (_elements.size() > 0) {
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 _elements.size() * sizeof(decltype(_elements)::value_type),
                 _elements.data(), GL_STATIC_DRAW);
  }

  return true;
}

bool PathVertices::doDraw(size_t index) const {
  glVertexAttribPointer(index, kVertexSize, GL_FLOAT, GL_FALSE, 0, nullptr);

  glEnableVertexAttribArray(index);

  switch (_type) {
    case PathVertices::Type::Fill: {
      GLenum mode = RL_CONSOLE_GET_VALUE_ONCE("Show Path Mesh", false)
                        ? GL_TRIANGLES
                        : GL_LINES;
      glDrawElements(mode, _elements.size(), GL_UNSIGNED_SHORT, nullptr);
    } break;
    case PathVertices::Type::Stroke: {
      glDrawArrays(GL_LINE_STRIP, 0, _vertices.size());
    } break;
  }

  glDisableVertexAttribArray(index);

  return true;
}

}  // namespace compositor
}  // namespace rl
