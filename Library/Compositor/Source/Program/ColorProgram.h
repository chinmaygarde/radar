// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_COMPOSITOR_PROGRAM_COLORPROGRAM_H_
#define RADAR_COMPOSITOR_PROGRAM_COLORPROGRAM_H_

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

  unsigned int modelViewProjectionUniform() const;

  unsigned int contentColorUniform() const;

  unsigned int sizeUniform() const;

 protected:
  void onLinkSuccess() override;

 private:
  unsigned int _modelViewProjectionUniform;
  unsigned int _contentColorUniform;
  unsigned int _sizeUniform;

  RL_DISALLOW_COPY_AND_ASSIGN(ColorProgram);
};

}  // namespace compositor
}  // namespace rl

#endif  // RADAR_COMPOSITOR_PROGRAM_COLORPROGRAM_H_
