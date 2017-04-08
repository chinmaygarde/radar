/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include "Program/Program.h"

namespace rl {
namespace compositor {

class TextureProgram : public Program {
 public:
  TextureProgram();

  GLint modelViewProjectionUniform() const;

  GLint sizeUniform() const;

  GLint alphaUniform() const;

  GLint textureUniform() const;

  GLint positionAttribute() const;

 protected:
  void onLinkSuccess() override;

 private:
  GLint _modelViewProjectionUniform;
  GLint _sizeUniform;
  GLint _alphaUniform;
  GLint _textureUniform;
  GLint _positionAttribute;

  RL_DISALLOW_COPY_AND_ASSIGN(TextureProgram);
};

}  // namespace compositor
}  // namespace rl
