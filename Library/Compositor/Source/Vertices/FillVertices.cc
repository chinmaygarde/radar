/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <libtess2/tesselator.h>
#include "Console.h"
#include "FillVertices.h"

namespace rl {
namespace compositor {

const int kVertexSize = 2;
const int kPolygonSize = 3;

static int ToTessWindingRule(FillVertices::Winding winding) {
  switch (winding) {
    case FillVertices::Winding::Odd:
      return TESS_WINDING_ODD;
    case FillVertices::Winding::NonZero:
      return TESS_WINDING_NONZERO;
    case FillVertices::Winding::Positive:
      return TESS_WINDING_POSITIVE;
    case FillVertices::Winding::Negative:
      return TESS_WINDING_NEGATIVE;
    case FillVertices::Winding::AbsGeqTwo:
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
  std::vector<gl::GLPoint> contours;
  geom::SmoothingApproximation defaultApproximation;
  path.smoothPoints(
      [&contours, pathSize = boundingBox.size ](std::vector<geom::Point> points)
          ->bool {
            for (const auto& point : points) {
              contours.emplace_back(point.x / pathSize.width,
                                    point.y / pathSize.height);
            }
            return true;
          },
      defaultApproximation);

  if (contours.size() == 0) {
    return {false, {}};
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

FillVertices::FillVertices(const geom::Path& path, Winding winding)
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
  std::tie(success, _size) = PopulateFillWithPath(tessellator.get(), path);

  if (!success) {
    return;
  }

  /*
   *  Perform tessellation.
   */
  auto result = tessTesselate(tessellator.get(),           // tessellator
                              ToTessWindingRule(winding),  // winding
                              TESS_POLYGONS,               // element type
                              kPolygonSize,                // polygon size
                              kVertexSize,                 // vertex size
                              nullptr                      // normal
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

FillVertices::~FillVertices() = default;

const geom::Size& FillVertices::size() const {
  return _size;
}

bool FillVertices::uploadVertexData() {
  glBufferData(GL_ARRAY_BUFFER,
               _vertices.size() * sizeof(decltype(_vertices)::value_type),
               _vertices.data(), GL_STATIC_DRAW);

  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               _elements.size() * sizeof(decltype(_elements)::value_type),
               _elements.data(), GL_STATIC_DRAW);

  return true;
}

bool FillVertices::draw(size_t positionAttributeIndex) const {
  auto bound = bindBuffer();

  if (!bound) {
    return false;
  }

  auto autoDisable =
      enableAttribute(positionAttributeIndex, kVertexSize, GL_FLOAT, 0, 0);

  const GLenum mode = RL_CONSOLE_GET_VALUE_ONCE("Show Path Mesh", false)
                          ? GL_LINE_LOOP
                          : GL_TRIANGLES;

  glDrawElements(mode, _elements.size(), GL_UNSIGNED_SHORT, nullptr);

  return true;
}

}  // namespace compositor
}  // namespace rl
