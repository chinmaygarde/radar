// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

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
