/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Compositor/BackendPass.h>
#include "ProgramCatalog.h"
#include "Texture.h"
#include "TexturedBoxPrimitive.h"
#include "Uniform.h"
#include "Vertices/BoxVertices.h"

namespace rl {
namespace compositor {

TexturedBoxPrimitive::TexturedBoxPrimitive(image::Image image)
    : _texture(std::make_shared<Texture>(std::move(image))) {}

TexturedBoxPrimitive::~TexturedBoxPrimitive() = default;

bool TexturedBoxPrimitive::prepareToRender(BackEndPass& backEndPass) {
  _texture = backEndPass.prepareTexture(_texture);
  return _texture != nullptr;
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
