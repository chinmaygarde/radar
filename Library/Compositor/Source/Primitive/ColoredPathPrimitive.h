/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Geometry/Path.h>
#include "Primitive.h"
#include "Vertices/PathVertices.h"

namespace rl {
namespace compositor {

class ColoredPathPrimitive : public Primitive {
 public:
  ColoredPathPrimitive(entity::Color color, const geom::Path& path);

  ~ColoredPathPrimitive() override;

 private:
  PathVertices _vertices;
  entity::Color _color;

  bool prepareToRender(BackEndPass& backEndPass) override;

  bool render(Frame& frame) const override;

  RL_DISALLOW_COPY_AND_ASSIGN(ColoredPathPrimitive);
};

}  // namespace compositor
}  // namespace rl
