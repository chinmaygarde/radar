/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include "Program/Program.h"

namespace rl {
namespace compositor {

/*
 *  The program to be used for drawing simple colored primitives
 */
class ColorProgram : public Program {
 public:
  ColorProgram();

  GLint modelViewProjectionUniform() const;

  GLint contentColorUniform() const;

  GLint sizeUniform() const;

  GLint positionAttribute() const;

 private:
  GLint _modelViewProjectionUniform;
  GLint _contentColorUniform;
  GLint _sizeUniform;
  GLint _positionAttribute;

  void onLinkSuccess() override;

  RL_DISALLOW_COPY_AND_ASSIGN(ColorProgram);
};

}  // namespace compositor
}  // namespace rl
