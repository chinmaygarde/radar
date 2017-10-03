/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>

namespace rl {
namespace bodymovin {

class Transform {
 public:
  Transform();

  ~Transform();

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(Transform);
};

}  // namespace bodymovin
}  // namespace rl
