// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/Primitive/TexturedBoxPrimitive.h>

namespace rl {
namespace compositor {

TexturedBoxPrimitive::TexturedBoxPrimitive() = default;

TexturedBoxPrimitive::~TexturedBoxPrimitive() = default;

bool TexturedBoxPrimitive::render(Frame& frame) const {
  return true;
}

void TexturedBoxPrimitive::setImage(image::Image image) {
  _image = std::move(image);
}

}  // namespace compositor
}  // namespace rl
