// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_COMPOSITOR_BACKENDPASS_H_
#define RADAR_COMPOSITOR_BACKENDPASS_H_

#include <Core/Macros.h>
#include <Compositor/FrontendPass.h>
#include <Compositor/Frame.h>
#include <Image/Image.h>

#include <vector>

namespace rl {
namespace compositor {

class TextureTransaction;

class BackEndPass {
 public:
  BackEndPass(std::vector<FrontEndPass> frontEndPasses);

  ~BackEndPass();

  bool render(core::WorkQueue& preparationWQ, Frame& frame);

  bool addImageForPreparation(image::Image image);

 private:
  std::vector<FrontEndPass> _frontEndPasses;
  std::unique_ptr<TextureTransaction> _textureTransaction;

  RL_DISALLOW_COPY_AND_ASSIGN(BackEndPass);
};

}  // namespace compositor
}  // namespace rl

#endif  // RADAR_COMPOSITOR_BACKENDPASS_H_
