/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <GLFoundation/GLFoundation.h>
#include <Geometry/Path.h>
#include "Vertices/Vertices.h"

namespace rl {
namespace compositor {

class PathVertices : public Vertices {
 public:
  enum class Winding {
    Odd,
    NonZero,
    Positive,
    Negative,
    AbsGeqTwo,
  };

  enum class ElementType {
    Polygons,
    ConnectedPolygons,
    BoundaryContours,
  };

  PathVertices(const geom::Path& path,
               Winding winding,
               ElementType elementType);

  ~PathVertices();

  const geom::Size& size() const;

  bool hasVerticesAndElements() const;

 private:
  geom::Size _size;
  const ElementType _type;
  std::vector<gl::GLPoint> _vertices;
  std::vector<GLshort> _elements;

  bool doDraw(size_t index) const override;

  bool uploadVertexData() override;

  RL_DISALLOW_COPY_AND_ASSIGN(PathVertices);
};

}  // namespace compositor
}  // namespace rl
