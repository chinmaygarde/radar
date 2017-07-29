/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Entity/Color.h>
#include <Geometry/Path.h>
#include "Primitive.h"
#include "Vertices/StrokeVertices.h"

namespace rl {
namespace compositor {

class ColoredPathStrokePrimitive : public Primitive {
 public:
  ColoredPathStrokePrimitive(const geom::Path& path,
                             entity::Color color,
                             double size);

  ~ColoredPathStrokePrimitive() override;

  bool prepareToRender(BackEndPass& backEndPass) override;

  bool render(Frame& frame) const override;

 private:
  StrokeVertices _vertices;
  const entity::Color _color;
  double _strokeSize;

  RL_DISALLOW_COPY_AND_ASSIGN(ColoredPathStrokePrimitive);
};

}  // namespace compositor
}  // namespace rl
