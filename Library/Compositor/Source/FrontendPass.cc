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

void FrontEndPass::addPrimitive(Primitive primitive) {
  _primitives.emplace_back(std::move(primitive));
}

void FrontEndPass::prepareInBackendPass(BackEndPass& pass) {}

bool FrontEndPass::render(const BackEndPass& pass, Frame& frame) const {
  for (const auto& primitive : _primitives) {
    if (!primitive.render(frame)) {
      return false;
    }
  }

  return true;
}

}  // namespace compositor
}  // namespace rl
