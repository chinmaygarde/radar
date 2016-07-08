// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/Primitive/TexturedBoxPrimitive.h>

#include "Texture.h"
#include <Compositor/BackendPass.h>

namespace rl {
namespace compositor {

TexturedBoxPrimitive::TexturedBoxPrimitive() = default;

TexturedBoxPrimitive::~TexturedBoxPrimitive() = default;

void TexturedBoxPrimitive::setImage(image::Image image) {
  _texture = core::make_unique<Texture>(std::move(image));
}

void TexturedBoxPrimitive::prepareToRender(BackEndPass& backEndPass) const {
  if (_texture == nullptr) {
    return;
  }

  backEndPass.prepareTexture(*_texture);
}

bool TexturedBoxPrimitive::render(Frame& frame) const {
  return true;
}

}  // namespace compositor
}  // namespace rl
