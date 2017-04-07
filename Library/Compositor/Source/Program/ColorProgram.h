// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

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
