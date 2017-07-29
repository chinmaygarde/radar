/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Geometry/Rect.h>
#include "Vertices/Vertices.h"

namespace rl {
namespace compositor {

class BoxVertices : public Vertices {
 public:
  BoxVertices(geom::Rect boxRect);

  bool draw(size_t positionAttributeIndex) const;

 private:
  const geom::Rect _boxRect;

  bool uploadVertexData() override;

  RL_DISALLOW_COPY_AND_ASSIGN(BoxVertices);
};

}  // namespace compositor
}  // namespace rl
