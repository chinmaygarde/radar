/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Geometry/Vector.h>
#include "Console.h"
#include "StrokeVertices.h"

namespace rl {
namespace compositor {

StrokeVertices::StrokeVertices(const geom::Path& path)
    : Vertices(Vertices::Type::Array) {
  _size = path.boundingBox().size;

  if (_size.isZero()) {
    return;
  }

  geom::SmoothingApproximation defaultApproximation;
  path.smoothPoints(std::bind(&StrokeVertices::tessellatePathComponent, this,
                              std::placeholders::_1),
                    defaultApproximation);
}

StrokeVertices::~StrokeVertices() = default;

bool StrokeVertices::tessellatePathComponent(
    const std::vector<geom::Point>& points) {
  if (points.size() < 2) {
    /*
     *  Not a failure. We can still keep tessellating other path components.
     */
    return true;
  }

  for (size_t i = 0, length = points.size(); i < length; i++) {
    const bool lastPoint = i == length - 1;

    const geom::Point& p1 = points[i];
    const geom::Point& p2 = lastPoint ? points[i - 1] : points[i + 1];

    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;

    const gl::GLPoint vertex(p1.x / _size.width, p1.y / _size.height);

    const double direction = lastPoint ? -1.0 : 1.0;

    const auto normal =
        geom::Vector3{-dy * direction, dx * direction}.normalize();

    if (i == 0) {
      _vertices.emplace_back(vertex, -normal, 0.0);
      _vertices.emplace_back(vertex, normal, 0.0);
    }

    _vertices.emplace_back(vertex, normal, 1.0);
    _vertices.emplace_back(vertex, -normal, 1.0);
  }

  return true;
}

const geom::Size& StrokeVertices::size() const {
  return _size;
}

bool StrokeVertices::setSize(const geom::Size& size) {
  if (size.isZero()) {
    return false;
  }
  _size = size;
  return true;
}

bool StrokeVertices::uploadVertexData() {
  glBufferData(GL_ARRAY_BUFFER,
               _vertices.size() * sizeof(decltype(_vertices)::value_type),
               _vertices.data(), GL_STATIC_DRAW);

  return true;
}

bool StrokeVertices::draw(size_t positionAttributeIndex,
                          size_t normalAttributeIndex,
                          size_t segmentContinuationAttributeIndex) const {
  auto bound = bindBuffer();

  if (!bound) {
    return false;
  }

  auto autoDisablePosition =
      enableAttribute(positionAttributeIndex, 2, GL_FLOAT, sizeof(StrokeVertex),
                      offsetof(StrokeVertex, position));

  auto autoDisableNormal =
      enableAttribute(normalAttributeIndex, 2, GL_FLOAT, sizeof(StrokeVertex),
                      offsetof(StrokeVertex, normal));

  auto autoDisableOpacity = enableAttribute(
      segmentContinuationAttributeIndex, 1, GL_FLOAT, sizeof(StrokeVertex),
      offsetof(StrokeVertex, segmentContinuation));

  glDrawArrays(RL_CONSOLE_GET_VALUE_ONCE("Show Stroke Mesh", false)
                   ? GL_LINE_STRIP
                   : GL_TRIANGLE_STRIP,
               0, _vertices.size());

  return true;
}

}  // namespace compositor
}  // namespace rl
