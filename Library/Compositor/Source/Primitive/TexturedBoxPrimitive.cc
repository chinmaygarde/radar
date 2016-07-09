// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/Primitive/TexturedBoxPrimitive.h>
#include <Compositor/BackendPass.h>

#include "Texture.h"
#include "ProgramCatalog.h"

namespace rl {
namespace compositor {

TexturedBoxPrimitive::TexturedBoxPrimitive() = default;

TexturedBoxPrimitive::~TexturedBoxPrimitive() = default;

void TexturedBoxPrimitive::setTextureImage(image::Image image) {
  _texture = std::make_shared<Texture>(std::move(image));
}

void TexturedBoxPrimitive::prepareToRender(BackEndPass& backEndPass) {
  if (_texture == nullptr) {
    return;
  }

  _texture = backEndPass.prepareTexture(_texture);
}

bool TexturedBoxPrimitive::render(Frame& frame) const {
  auto& program = frame.context().programCatalog().textureProgram();

  if (!program.use()) {
    return false;
  }

  return true;
}

}  // namespace compositor
}  // namespace rl
