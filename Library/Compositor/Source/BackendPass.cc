// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/BackendPass.h>
#include "TextureTransaction.h"

namespace rl {
namespace compositor {

BackEndPass::BackEndPass(std::vector<FrontEndPass> frontEndPasses)
    : _frontEndPasses(std::move(frontEndPasses)),
      _textureTransaction(core::make_unique<TextureTransaction>()) {}

BackEndPass::~BackEndPass() = default;

bool BackEndPass::render(core::WorkQueue& preparationWQ, Frame& frame) {
  if (_frontEndPasses.size() == 0) {
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
    RL_RETURN_IF_FALSE(frontEndPass.render(*this, frame));
    primitivesStatistics.increment(frontEndPass.primitivesCount());
  }

  return true;
}

bool BackEndPass::addImageForPreparation(image::Image image) {
  return _textureTransaction->addImage(std::move(image));
}

}  // namespace compositor
}  // namespace rl
