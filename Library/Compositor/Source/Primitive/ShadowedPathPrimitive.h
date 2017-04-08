/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include "Primitive.h"

namespace rl {
namespace compositor {

class ShadowedPathPrimitive : public Primitive {
 public:
  ShadowedPathPrimitive();

  ~ShadowedPathPrimitive() override;

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(ShadowedPathPrimitive);
};

}  // namespace compositor
}  // namespace rl
