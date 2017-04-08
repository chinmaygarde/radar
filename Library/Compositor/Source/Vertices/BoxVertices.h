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

 private:
  const geom::Rect _boxRect;

  bool doDraw(size_t index) override;
  bool uploadVertexData() override;

  RL_DISALLOW_COPY_AND_ASSIGN(BoxVertices);
};

}  // namespace compositor
}  // namespace rl
