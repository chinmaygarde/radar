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
  GLint _modelViewProjectionUniform = -1;
  GLint _sizeUniform = -1;
  GLint _alphaUniform = -1;
  GLint _textureUniform = -1;
  GLint _positionAttribute = -1;

  RL_DISALLOW_COPY_AND_ASSIGN(TextureProgram);
};

}  // namespace compositor
}  // namespace rl
