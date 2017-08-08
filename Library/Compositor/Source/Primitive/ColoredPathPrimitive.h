/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Geometry/Path.h>
#include "Primitive.h"
#include "Vertices/FillVertices.h"

namespace rl {
namespace compositor {

class ColoredPathPrimitive : public Primitive {
 public:
  ColoredPathPrimitive(const geom::Path& path);

  ~ColoredPathPrimitive() override;

 private:
  FillVertices _vertices;

  bool prepareToRender(BackEndPass& backEndPass) override;

  bool render(Frame& frame) const override;

  RL_DISALLOW_COPY_AND_ASSIGN(ColoredPathPrimitive);
};

}  // namespace compositor
}  // namespace rl
