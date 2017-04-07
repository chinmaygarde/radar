// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <Core/Macros.h>
#include <Geometry/Path.h>
#include "OpenGL.h"
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

  std::vector<GLPoint> _vertices;
  std::vector<GLshort> _elements;

  bool doDraw(size_t index) override;
  bool uploadVertexData() override;

  RL_DISALLOW_COPY_AND_ASSIGN(PathVertices);
};

}  // namespace compositor
}  // namespace rl
