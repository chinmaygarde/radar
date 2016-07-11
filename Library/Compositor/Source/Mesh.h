// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_COMPOSITOR_MESH_H_
#define RADAR_COMPOSITOR_MESH_H_

#include <Core/Macros.h>

#include <Geometry/Path.h>

#include "OpenGL.h"

namespace rl {
namespace compositor {

class Mesh {
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

  Mesh(const geom::Path& path, Winding winding, ElementType elementType);

  ~Mesh();

 private:
  std::vector<GLPoint> _vertices;
  std::vector<GLshort> _elements;

  RL_DISALLOW_COPY_AND_ASSIGN(Mesh);
};

}  // namespace compositor
}  // namespace rl

#endif  // RADAR_COMPOSITOR_MESH_H_
