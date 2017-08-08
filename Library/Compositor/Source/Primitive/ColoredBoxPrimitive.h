/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include "Primitive.h"

namespace rl {
namespace compositor {

class ColoredBoxPrimitive : public Primitive {
 public:
  ColoredBoxPrimitive();

  ~ColoredBoxPrimitive() override;

  bool prepareToRender(BackEndPass& backEndPass) override;

  bool render(Frame& frame) const override;

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(ColoredBoxPrimitive);
};

}  // namespace compositor
}  // namespace rl
