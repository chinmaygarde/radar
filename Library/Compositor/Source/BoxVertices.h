// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_COMPOSITOR_BOXVERTICES_H_
#define RADAR_COMPOSITOR_BOXVERTICES_H_

#include "Vertices.h"

#include <Geometry/Rect.h>

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

#endif  // RADAR_COMPOSITOR_BOXVERTICES_H_
