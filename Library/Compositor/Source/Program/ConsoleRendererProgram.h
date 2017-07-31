/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include "Program/Program.h"

namespace rl {
namespace compositor {

class ConsoleRendererProgram : public Program {
 public:
  ConsoleRendererProgram();

  ~ConsoleRendererProgram();

  GLint textureUniform() const;

  GLint projectionMatrixUniform() const;

  GLint positionAttribute() const;

  GLint uvAttribute() const;

  GLint colorAttribute() const;

 protected:
  GLint _textureUniform = -1;
  GLint _projectionMatrixUniform = -1;
  GLint _positionAttribute = -1;
  GLint _uvAttribute = -1;
  GLint _colorAttribute = -1;

  void onLinkSuccess() override;

  RL_DISALLOW_COPY_AND_ASSIGN(ConsoleRendererProgram);
};

}  // namespace compositor
}  // namespace rl
