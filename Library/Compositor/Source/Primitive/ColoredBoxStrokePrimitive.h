/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Entity/Color.h>
#include "Primitive.h"

namespace rl {
namespace compositor {

class ColoredBoxStrokePrimitive : public Primitive {
 public:
  ColoredBoxStrokePrimitive(entity::Color color, double size);

  ~ColoredBoxStrokePrimitive() override;

  bool prepareToRender(BackEndPass& backEndPass) override;

  bool render(Frame& frame) const override;

 private:
  const entity::Color _color;
  const double _size;

  RL_DISALLOW_COPY_AND_ASSIGN(ColoredBoxStrokePrimitive);
};

}  // namespace compositor
}  // namespace rl
