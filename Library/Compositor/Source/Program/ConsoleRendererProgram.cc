/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "ConsoleRendererProgram.h"

namespace rl {
namespace compositor {

static const char RendererVertexShader[] = R"--(
  uniform mat4 ProjMtx;

  attribute vec2 Position;
  attribute vec2 UV;
  attribute vec4 Color;

  varying vec2 Frag_UV;
  varying vec4 Frag_Color;

  void main() {
    Frag_UV = UV;
    Frag_Color = Color;
    gl_Position = ProjMtx * vec4(Position.xy, 0, 1);
  }
  )--";

static const char RendererFragmentShader[] = R"--(

#ifdef GL_ES
  precision mediump float;
#endif

  uniform sampler2D Texture;

  varying vec2 Frag_UV;
  varying vec4 Frag_Color;

  void main() {
    gl_FragColor = Frag_Color * texture2D(Texture, Frag_UV.st);
  }
  )--";

ConsoleRendererProgram::ConsoleRendererProgram()
    : Program(RendererVertexShader, RendererFragmentShader) {}

ConsoleRendererProgram::~ConsoleRendererProgram() = default;

void ConsoleRendererProgram::onLinkSuccess() {
  textureUniform = indexForUniform("Texture");
  projMtxUniform = indexForUniform("ProjMtx");

  positionAttribute = indexForAttribute("Position");
  uvAttribute = indexForAttribute("UV");
  colorAttribute = indexForAttribute("Color");
}

}  // namespace compositor
}  // namespace rl
