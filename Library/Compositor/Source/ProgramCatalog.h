// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_COMPOSITOR_PROGRAMCATALOG_
#define RADARLOVE_COMPOSITOR_PROGRAMCATALOG_

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

#endif  // RADARLOVE_COMPOSITOR_PROGRAMCATALOG_
