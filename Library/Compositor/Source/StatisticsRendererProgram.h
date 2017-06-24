/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include "Program/Program.h"

namespace rl {
namespace compositor {

class StatisticsRendererProgram : public Program {
 public:
  StatisticsRendererProgram();

  ~StatisticsRendererProgram();

  unsigned int textureUniform;

  unsigned int projMtxUniform;

  unsigned int positionAttribute;

  unsigned int uvAttribute;

  unsigned int colorAttribute;

 protected:
  void onLinkSuccess() override;

  RL_DISALLOW_COPY_AND_ASSIGN(StatisticsRendererProgram);
};

}  // namespace compositor
}  // namespace rl
