/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Geometry/Path.h>
#include "Vertices/Vertices.h"

namespace rl {
namespace compositor {

class StrokeVertices : public Vertices {
 public:
  StrokeVertices(const geom::Path& path);

  ~StrokeVertices() override;

  const geom::Size& size() const;

  bool draw(size_t positionAttributeIndex,
            size_t normalAttributeIndex,
            size_t opacityAttributeIndex) const;

 private:
  geom::Size _size;

  struct StrokeVertex {
    gl::GLPoint position;
    gl::GLPoint normal;
    GLfloat segmentContinuation;

    StrokeVertex(gl::GLPoint pPosition,
                 gl::GLPoint pNormal,
                 GLfloat pSegmentContinuation)
        : position(pPosition),
          normal(pNormal),
          segmentContinuation(pSegmentContinuation) {}
  };

  std::vector<StrokeVertex> _vertices;

  bool uploadVertexData() override;

  bool tessellatePathComponent(const std::vector<geom::Point>& points);

  RL_DISALLOW_COPY_AND_ASSIGN(StrokeVertices);
};

}  // namespace compositor
}  // namespace rl
