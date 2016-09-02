// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "TexturedPathPrimitive.h"

#include <Compositor/BackendPass.h>

#include "Uniform.h"
#include "Texture.h"
#include "Vertices/PathVertices.h"
#include "ProgramCatalog.h"

namespace rl {
namespace compositor {

TexturedPathPrimitive::TexturedPathPrimitive(image::Image image,
                                             const geom::Path& path)
    : _texture(std::make_shared<Texture>(std::move(image))),
      _pathVertices(core::make_unique<PathVertices>(
          path,
          PathVertices::Winding::Odd,
          PathVertices::ElementType::Polygons)) {}

TexturedPathPrimitive::~TexturedPathPrimitive() = default;

void TexturedPathPrimitive::prepareToRender(BackEndPass& backEndPass) {
  if (_texture == nullptr) {
    return;
  }

  _texture = backEndPass.prepareTexture(_texture);
}

bool TexturedPathPrimitive::render(Frame& frame) const {
  /*
   *  Select the program to use
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
  SetUniform(program.sizeUniform(), _pathVertices->size());
  SetUniform(program.alphaUniform(), _opacity);

  /**
   *  Draw vertices.
   */
  return _pathVertices->draw(program.positionAttribute());
}

}  // namespace compositor
}  // namespace rl
