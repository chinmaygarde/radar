// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/CompositorStatisticsRenderer.h>
#include <Compositor/Program.h>
#include <imgui/imgui.h>

#define RL_OPENGL_ALLOWED 1
#include <Compositor/OpenGL.h>

namespace rl {

static const std::string RendererVertexShader = R"--(
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

static const std::string RendererFragmentShader = R"--(

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

class CompositorStatisticsRendererProgram : public Program {
 public:
  CompositorStatisticsRendererProgram()
      : Program({"Position", "UV", "Color"},
                RendererVertexShader,
                RendererFragmentShader) {}

  unsigned int textureUniform;
  unsigned int projMtxUniform;
  unsigned int positionAttribute;
  unsigned int uvAttribute;
  unsigned int colorAttribute;

 protected:
  void onLinkSuccess() override {
    textureUniform = indexForUniform("Texture");
    projMtxUniform = indexForUniform("ProjMtx");

    positionAttribute = indexForAttribute("Position");
    uvAttribute = indexForAttribute("UV");
    colorAttribute = indexForAttribute("Color");
  }

  RL_DISALLOW_COPY_AND_ASSIGN(CompositorStatisticsRendererProgram);
};

static CompositorStatisticsRenderer* _CompositorStatisticsRenderer = nullptr;
void CompositorStatisticsRenderer::drawLists(void* data) {
  RL_ASSERT(_CompositorStatisticsRenderer != nullptr);
  CompositorStatisticsRenderer& renderer = *_CompositorStatisticsRenderer;
  auto drawData = static_cast<ImDrawData*>(data);

  /*
   *  Setup render state: alpha-blending enabled, no face culling, no depth
   *  testing, scissor enabled
   */
  glEnable(GL_BLEND);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_SCISSOR_TEST);
  glActiveTexture(GL_TEXTURE0);

  /*
   *  Setup orthographic projection matrix
   */
  auto& io = ImGui::GetIO();
  const GLfloat width = io.DisplaySize.x;
  const GLfloat height = io.DisplaySize.y;

  // clang-format off
  const GLfloat orthoProjection[4][4] = {
      { 2.0f / width, 0.0f,            0.0f, 0.0f },
      { 0.0f,         2.0f / -height,  0.0f, 0.0f },
      { 0.0f,         0.0f,           -1.0f, 0.0f },
      { -1.0f,        1.0f,            0.0f, 1.0f },
    };
  // clang-format on

  /*
   *  Setup program and update uniforms and vertices
   */
  auto& program = *renderer._program;

  program.use();

  glUniform1i(program.textureUniform, 0);
  glUniformMatrix4fv(program.projMtxUniform, 1, GL_FALSE,
                     &orthoProjection[0][0]);

  glBindBuffer(GL_ARRAY_BUFFER, renderer._vbo);

  glEnableVertexAttribArray(program.positionAttribute);
  glEnableVertexAttribArray(program.uvAttribute);
  glEnableVertexAttribArray(program.colorAttribute);

  glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, GL_FALSE,
                        sizeof(ImDrawVert),
                        reinterpret_cast<GLvoid*>(offsetof(ImDrawVert, pos)));
  glVertexAttribPointer(program.uvAttribute, 2, GL_FLOAT, GL_FALSE,
                        sizeof(ImDrawVert),
                        reinterpret_cast<GLvoid*>(offsetof(ImDrawVert, uv)));
  glVertexAttribPointer(program.colorAttribute, 4, GL_UNSIGNED_BYTE, GL_TRUE,
                        sizeof(ImDrawVert),
                        reinterpret_cast<GLvoid*>(offsetof(ImDrawVert, col)));

  for (int n = 0; n < drawData->CmdListsCount; n++) {
    ImDrawList* cmdList = drawData->CmdLists[n];
    ImDrawIdx* idxBuffer = &cmdList->IdxBuffer.front();

    glBufferData(GL_ARRAY_BUFFER,
                 (GLsizeiptr)cmdList->VtxBuffer.size() * sizeof(ImDrawVert),
                 &cmdList->VtxBuffer[0], GL_STREAM_DRAW);

    for (const ImDrawCmd* pCmd = cmdList->CmdBuffer.begin();
         pCmd != cmdList->CmdBuffer.end(); pCmd++) {
      if (pCmd->UserCallback) {
        pCmd->UserCallback(cmdList, pCmd);
      } else {
        glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pCmd->TextureId);

        glScissor(static_cast<GLint>(pCmd->ClipRect.x),
                  static_cast<GLint>(height - pCmd->ClipRect.w),
                  static_cast<GLsizei>(pCmd->ClipRect.z - pCmd->ClipRect.x),
                  static_cast<GLsizei>(pCmd->ClipRect.w - pCmd->ClipRect.y));

        glDrawElements(GL_TRIANGLES, (GLsizei)pCmd->ElemCount,
                       GL_UNSIGNED_SHORT, idxBuffer);
      }

      idxBuffer += pCmd->ElemCount;
    }
  }

  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

CompositorStatisticsRenderer::CompositorStatisticsRenderer()
    : _setupComplete(false),
      _program(nullptr),
      _vbo(GL_NONE),
      _fontAtlas(GL_NONE) {
  auto& io = ImGui::GetIO();
  io.RenderDrawListsFn = reinterpret_cast<void (*)(ImDrawData* data)>(
      &CompositorStatisticsRenderer::drawLists);
}

CompositorStatisticsRenderer::~CompositorStatisticsRenderer() {
  cleanup();
}

void CompositorStatisticsRenderer::cleanup() {
  /*
   *  GL_NONEs are ignored
   */
  glDeleteBuffers(1, &_vbo);
  glDeleteTextures(1, &_fontAtlas);
}

void CompositorStatisticsRenderer::performSetupIfNecessary() {
  if (_setupComplete) {
    return;
  }

  _setupComplete = true;

  /*
   *  Create and initialize the shader program
   */
  _program = rl::make_unique<CompositorStatisticsRendererProgram>();

  RL_GLAssert("There must be no errors prior to stat renderer setup");

  /*
   *  Create the vertex buffer. No VAOs will be used
   */
  glGenBuffers(1, &_vbo);

  /*
   *  Create and initialize font atlases
   */
  glGenTextures(1, &_fontAtlas);
  glBindTexture(GL_TEXTURE_2D, _fontAtlas);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  GLvoid* pixels = nullptr;
  int fontAtlasWidth = 0;
  int fontAtlasHeight = 0;
  auto& io = ImGui::GetIO();

  io.Fonts->GetTexDataAsRGBA32(reinterpret_cast<unsigned char**>(&pixels),
                               &fontAtlasWidth, &fontAtlasHeight);

  RL_ASSERT(pixels != nullptr && fontAtlasWidth != 0 && fontAtlasHeight != 0);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fontAtlasWidth, fontAtlasHeight, 0,
               GL_RGBA, GL_UNSIGNED_BYTE, pixels);

  io.Fonts->TexID = reinterpret_cast<void*>((intptr_t)_fontAtlas);
  io.Fonts->ClearInputData();
  io.Fonts->ClearTexData();

  RL_GLAssert("There must be no errors post stat renderer setup");
}

void CompositorStatisticsRenderer::render(CompositorStatistics& stats,
                                          Frame& frame) {
  performSetupIfNecessary();

  auto& io = ImGui::GetIO();
  auto& size = frame.size();
  io.DisplaySize.x = size.width;
  io.DisplaySize.y = size.height;

  /*
   *  The framework does not allow for passing user pointers to the draw
   *  callback function. Since the stats renderer is a singleton anyway, we set
   *  a static global for the duration of the call.
   */
  _CompositorStatisticsRenderer = this;

  ImGui::NewFrame();

  ImGui::SetNextWindowSize(ImVec2(240, 160), ImGuiSetCond_FirstUseEver);
  ImGui::SetNextWindowPos(ImVec2(size.width - 260, 20), ImGuiSetCond_Always);

  buildStatsUI(stats);

  ImGui::Render();

  _CompositorStatisticsRenderer = nullptr;
}

void CompositorStatisticsRenderer::buildStatsUI(CompositorStatistics& stats) {
  if (ImGui::Begin("Compositor Statistics")) {
    ImGui::Text("Entities: %zu", stats.entityCount().count());
    ImGui::Text("Primitives: %zu", stats.primitiveCount().count());
    ImGui::Text("Frames Rendered: %zu", stats.frameCount().count());
    ImGui::Text("Interpolations (%zu): %.2f ms",
                stats.interpolationsCount().count(),
                stats.interpolations().lastLap().count() * 1e3);
    ImGui::Text("Last Transaction Update: %.2f ms",
                stats.transactionUpdateTimer().lastLap().count() * 1e3);

    ImGui::Text("Frame Time (minus swap):");
    auto frameMs = stats.frameTimer().currentLap().count() * 1e3;
    ImGui::Text("    %.2f ms (%.0f FPS)", frameMs, 1000.0 / frameMs);
  }
  ImGui::End();
}

}  // namespace rl
