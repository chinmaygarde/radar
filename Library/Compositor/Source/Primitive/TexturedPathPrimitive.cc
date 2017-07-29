/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Compositor/BackendPass.h>
#include "ProgramCatalog.h"
#include "TexturedPathPrimitive.h"
#include "Uniform.h"

namespace rl {
namespace compositor {

TexturedPathPrimitive::TexturedPathPrimitive(image::Image image,
                                             const geom::Path& path)
    : _vertices(path, FillVertices::Winding::Odd),
      _texture(std::make_shared<Texture>(std::move(image))) {}

TexturedPathPrimitive::~TexturedPathPrimitive() = default;

bool TexturedPathPrimitive::prepareToRender(BackEndPass& backEndPass) {
  _texture = backEndPass.prepareTexture(_texture);

  if (_texture == nullptr) {
    return false;
  }

  return _vertices.prepare();
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
  SetUniform(program.sizeUniform(), _vertices.size());
  SetUniform(program.alphaUniform(), _opacity);

  /**
   *  Draw vertices.
   */
  return _vertices.draw(program.positionAttribute());
}

}  // namespace compositor
}  // namespace rl
