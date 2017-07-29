/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Geometry/Path.h>
#include <vector>
#include "Vertices/Vertices.h"

namespace rl {
namespace compositor {

class FillVertices : public Vertices {
 public:
  enum class Winding {
    Odd,
    NonZero,
    Positive,
    Negative,
    AbsGeqTwo,
  };

  FillVertices(const geom::Path& path, Winding winding);

  ~FillVertices() override;

  const geom::Size& size() const;

  bool draw(size_t positionAttributeIndex) const;

 private:
  geom::Size _size;
  std::vector<gl::GLPoint> _vertices;
  std::vector<GLshort> _elements;

  bool uploadVertexData() override;

  RL_DISALLOW_COPY_AND_ASSIGN(FillVertices);
};

}  // namespace compositor
}  // namespace rl
