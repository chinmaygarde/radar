/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include "Primitive.h"

namespace rl {
namespace compositor {

class ShadowedBoxPrimitive : public Primitive {
 public:
  ShadowedBoxPrimitive();

  ~ShadowedBoxPrimitive() override;

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(ShadowedBoxPrimitive);
};

}  // namespace compositor
}  // namespace rl
