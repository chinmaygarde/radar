// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_COMPOSITOR_BACKENDPASS_H_
#define RADAR_COMPOSITOR_BACKENDPASS_H_

#include <Compositor/Frame.h>
#include <Compositor/FrontendPass.h>
#include <Core/Macros.h>
#include <Image/Image.h>
#include <vector>

namespace rl {
namespace compositor {

class TextureTransaction;
class Texture;

class BackEndPass {
 public:
  BackEndPass();

  ~BackEndPass();

  bool hasRenderables() const;

  void addFrontEndPass(FrontEndPass frontEndPass);

  bool render(Frame& frame, core::WorkQueue* preparationWQ);

  RL_WARN_UNUSED_RESULT
  std::shared_ptr<Texture> prepareTexture(std::shared_ptr<Texture> texture);

 private:
  std::vector<FrontEndPass> _frontEndPasses;
  std::unique_ptr<TextureTransaction> _textureTransaction;

  RL_DISALLOW_COPY_AND_ASSIGN(BackEndPass);
};

}  // namespace compositor
}  // namespace rl

#endif  // RADAR_COMPOSITOR_BACKENDPASS_H_
