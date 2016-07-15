// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/Primitive/TexturedBoxPrimitive.h>
#include <Compositor/BackendPass.h>

#include "Texture.h"
#include "ProgramCatalog.h"
#include "Uniform.h"
#include "Vertices/BoxVertices.h"

namespace rl {
namespace compositor {

TexturedBoxPrimitive::TexturedBoxPrimitive(image::Image image)
    : _texture(std::make_shared<Texture>(std::move(image))) {}

TexturedBoxPrimitive::~TexturedBoxPrimitive() = default;

void TexturedBoxPrimitive::prepareToRender(BackEndPass& backEndPass) {
  if (_texture == nullptr) {
    return;
  }

  _texture = backEndPass.prepareTexture(_texture);
}

bool TexturedBoxPrimitive::render(Frame& frame) const {
  /*
   *  Select the program to use.
   */
  auto& program = frame.context().programCatalog().textureProgram();

  if (!program.use()) {
    return false;
  }

  /*
   *  Bind texture.
   */
  if (!_texture || !_texture->bind(program.textureUniform())) {
    return false;
  }

  /*
   *  Set uniforms.
   */
  SetUniform(program.modelViewProjectionUniform(),
             frame.projectionMatrix() * _modelViewMatrix);
  SetUniform(program.sizeUniform(), _size);
  SetUniform(program.alphaUniform(), _opacity);

  /*
   *  Draw vertices.
   */
  return frame.context().unitBoxVertices().draw(program.positionAttribute());
}

}  // namespace compositor
}  // namespace rl
