/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Compositor/Frame.h>
#include <Compositor/FrontendPass.h>
#include <Core/Macros.h>
#include <Core/WorkQueue.h>
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
