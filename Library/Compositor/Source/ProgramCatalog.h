/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include "Program/ColorProgram.h"
#include "Program/Program.h"
#include "Program/TextureProgram.h"

namespace rl {
namespace compositor {

class ProgramCatalog {
 public:
  ProgramCatalog();

  ~ProgramCatalog();

  ColorProgram& colorProgram();

  TextureProgram& textureProgram();

 private:
  ColorProgram _colorProgram;
  TextureProgram _textureProgram;

  RL_DISALLOW_COPY_AND_ASSIGN(ProgramCatalog);
};

}  // namespace compositor
}  // namespace rl
