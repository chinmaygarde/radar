/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include "Primitive.h"

namespace rl {
namespace compositor {

class BorderedBoxPrimitive : public Primitive {
 public:
  BorderedBoxPrimitive();

  ~BorderedBoxPrimitive() override;

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(BorderedBoxPrimitive);
};

}  // namespace compositor
}  // namespace rl
