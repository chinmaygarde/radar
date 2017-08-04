/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include "Program.h"

namespace rl {
namespace compositor {

class StrokeProgram : public Program {
 public:
  StrokeProgram();

  ~StrokeProgram() override;

  GLint modelViewProjectionUniform() const;

  GLint contentColorUniform() const;

  GLint sizeUniform() const;

  GLint featherUniform() const;

  GLint strokeSizeUniform() const;

  GLint positionAttribute() const;

  GLint normalAttribute() const;

  GLint segmentContinuationAttribute() const;

 private:
  GLint _modelViewProjectionUniform = -1;
  GLint _contentColorUniform = -1;
  GLint _sizeUniform = -1;
  GLint _featherUniform = -1;
  GLint _strokeSizeUniform = -1;
  GLint _positionAttribute = -1;
  GLint _normalAttribute = -1;
  GLint _segmentContinuationAttribute = -1;

  void onLinkSuccess() override;

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(StrokeProgram);
};

}  // namespace compositor
}  // namespace rl
