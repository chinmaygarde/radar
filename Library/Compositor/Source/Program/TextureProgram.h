// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_COMPOSITOR_PROGRAM_TEXTUREPROGRAM_H_
#define RADAR_COMPOSITOR_PROGRAM_TEXTUREPROGRAM_H_

#include <Core/Macros.h>
#include "Program/Program.h"

namespace rl {
namespace compositor {

class TextureProgram : public Program {
 public:
  TextureProgram();

 protected:
  void onLinkSuccess() override;

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(TextureProgram);
};

}  // namespace compositor
}  // namespace rl

#endif  // RADAR_COMPOSITOR_PROGRAM_TEXTUREPROGRAM_H_
