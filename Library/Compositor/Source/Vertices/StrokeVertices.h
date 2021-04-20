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

  RL_WARN_UNUSED_RESULT
  bool setSize(const geom::Size& size);

  double totalPathDistance() const;

  bool draw(size_t positionAttributeIndex,
            size_t normalAttributeIndex,
            size_t opacityAttributeIndex,
            size_t completionDistanceAtributeIndex) const;

 private:
  geom::Size _size;
  double _totalDistance;

  struct StrokeVertex {
    gl::GLPoint position;
    gl::GLPoint normal;
    GLfloat segmentContinuation;
    GLfloat completionDistance;

    StrokeVertex(gl::GLPoint pPosition,
                 gl::GLPoint pNormal,
                 GLfloat pSegmentContinuation,
                 GLfloat pCompletionDistance)
        : position(pPosition),
          normal(pNormal),
          segmentContinuation(pSegmentContinuation),
          completionDistance(pCompletionDistance) {}
  };

  std::vector<StrokeVertex> _vertices;

  bool uploadVertexData() override;

  bool tessellatePathComponent(const std::vector<geom::Point>& points);

  RL_DISALLOW_COPY_AND_ASSIGN(StrokeVertices);
};

}  // namespace compositor
}  // namespace rl
