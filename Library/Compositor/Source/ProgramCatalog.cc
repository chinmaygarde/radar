/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "ProgramCatalog.h"

namespace rl {
namespace compositor {

ProgramCatalog::ProgramCatalog() = default;

ProgramCatalog::~ProgramCatalog() = default;

ColorProgram& ProgramCatalog::colorProgram() {
  return _colorProgram;
}

TextureProgram& ProgramCatalog::textureProgram() {
  return _textureProgram;
}

}  // namespace compositor
}  // namespace rl
