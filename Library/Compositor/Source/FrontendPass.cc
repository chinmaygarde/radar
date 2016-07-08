// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/FrontendPass.h>

namespace rl {
namespace compositor {

FrontEndPass::FrontEndPass() = default;

FrontEndPass::~FrontEndPass() = default;

FrontEndPass::FrontEndPass(FrontEndPass&&) = default;

size_t FrontEndPass::primitivesCount() const {
  return _primitives.size();
}

void FrontEndPass::addPrimitive(std::unique_ptr<Primitive> primitive) {
  RL_ASSERT(primitive != nullptr);

  _primitives.emplace_back(std::move(primitive));
}

bool FrontEndPass::prepareInBackendPass(BackEndPass& pass) {
  for (const auto& primitive : _primitives) {
    primitive->prepareToRender(pass);
  }

  return true;
}

bool FrontEndPass::render(const BackEndPass& pass, Frame& frame) const {
  for (const auto& primitive : _primitives) {
    RL_RETURN_IF_FALSE(primitive->render(frame));
  }

  return true;
}

}  // namespace compositor
}  // namespace rl
