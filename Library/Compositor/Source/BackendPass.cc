/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Compositor/BackendPass.h>
#include "TextureTransaction.h"

namespace rl {
namespace compositor {

BackEndPass::BackEndPass()
    : _textureTransaction(std::make_unique<TextureTransaction>()) {}

BackEndPass::~BackEndPass() = default;

bool BackEndPass::hasRenderables() const {
  return _frontEndPasses.size() > 0;
}

void BackEndPass::addFrontEndPass(FrontEndPass frontEndPass) {
  if (!frontEndPass.hasRenderables()) {
    return;
  }

  _frontEndPasses.emplace_back(std::move(frontEndPass));
}

bool BackEndPass::render(Frame& frame, core::WorkQueue* preparationWQ) {
  if (!hasRenderables()) {
    return false;
  }

  /*
   *  Give primitives a chance to set themselves up in this back-end pass.
   */
  for (auto& frontEndPass : _frontEndPasses) {
    frontEndPass.prepareInBackendPass(*this);
  }

  if (!_textureTransaction->commit(preparationWQ)) {
    return false;
  }

  /*
   *  Render all primitives in each pass.
   */
  auto& primitivesStatistics = frame.context().statistics().primitiveCount();

  for (const auto& frontEndPass : _frontEndPasses) {
    RL_RETURN_IF_FALSE(frontEndPass.renderInBackEndPass(*this, frame));
    primitivesStatistics.increment(frontEndPass.primitivesCount());
  }

  return true;
}

std::shared_ptr<Texture> BackEndPass::prepareTexture(
    std::shared_ptr<Texture> texture) {
  return _textureTransaction->registerTexture(texture);
}

}  // namespace compositor
}  // namespace rl
