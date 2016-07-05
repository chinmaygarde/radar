// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/FrontendPass.h>

namespace rl {
namespace compositor {

FrontEndPass::FrontEndPass() = default;

FrontEndPass::~FrontEndPass() = default;

FrontEndPass::FrontEndPass(FrontEndPass&&) = default;

void FrontEndPass::addPrimitive(Primitive primitive) {
  if (!primitive.isRenderable()) {
    return;
  }

  _primitives.emplace_back(std::move(primitive));
}

}  // namespace compositor
}  // namespace rl
