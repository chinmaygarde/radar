// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Utilities.h>
#include <imgui/imgui.h>
#include "OpenGL.h"
#include "Program/Program.h"
#include "StatisticsRenderer.h"

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

class StatisticsRendererProgram : public Program {
 public:
  StatisticsRendererProgram()
      : Program(RendererVertexShader, RendererFragmentShader) {}

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

  RL_DISALLOW_COPY_AND_ASSIGN(StatisticsRendererProgram);
};

static StatisticsRenderer* _StatisticsRenderer = nullptr;
void StatisticsRenderer::drawLists(void* data) {
  RL_ASSERT(_StatisticsRenderer != nullptr);
  StatisticsRenderer& renderer = *_StatisticsRenderer;

  /*
   *  Setup program and update uniforms and vertices
   */
  auto& program = *renderer._program;

  if (!program.use()) {
    return;
  }

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

  glDisableVertexAttribArray(program.positionAttribute);
  glDisableVertexAttribArray(program.uvAttribute);
  glDisableVertexAttribArray(program.colorAttribute);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

StatisticsRenderer::StatisticsRenderer()
    : _setupComplete(false),
      _program(nullptr),
      _vbo(GL_NONE),
      _fontAtlas(GL_NONE) {
  auto& io = ImGui::GetIO();
  io.RenderDrawListsFn = reinterpret_cast<void (*)(ImDrawData * data)>(
      &StatisticsRenderer::drawLists);
}

StatisticsRenderer::~StatisticsRenderer() {
  /*
   *  GL_NONEs are ignored but we don't even want to make the OpenGL call in
   *  case initialization has not yet taken place.
   */
  if (_vbo != GL_NONE) {
    glDeleteBuffers(1, &_vbo);
  }

  if (_fontAtlas != GL_NONE) {
    glDeleteTextures(1, &_fontAtlas);
  }
}

void StatisticsRenderer::performSetupIfNecessary() {
  if (_setupComplete) {
    return;
  }

  _setupComplete = true;

  /*
   *  Create and initialize the shader program
   */
  _program = std::make_unique<StatisticsRendererProgram>();

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

static void BuildStatsUI(CompositorStatistics& compositorStats) {
  if (ImGui::Begin("Coordinator Statistics")) {
    ImGui::Text("Entities: %zu", compositorStats.entityCount().count());
    ImGui::Text("Primitives: %zu", compositorStats.primitiveCount().count());
    ImGui::Text("Frames Rendered: %zu", compositorStats.frameCount().count());
    ImGui::Text("Frame Time (minus swap):");
    auto frameMs = compositorStats.frameTimer().currentLap().count() * 1e3;
    ImGui::Text("    %.2f ms (%.0f FPS)", frameMs, 1000.0 / frameMs);
  }
  ImGui::End();
}

#if 0

static void BuildStatsUI(InterfaceStatistics& interfaceStats) {
  if (ImGui::Begin(interfaceStats.tag().c_str())) {
    ImGui::Text("Interpolations (%zu): %.2f ms",
                interfaceStats.interpolationsCount().count(),
                interfaceStats.interpolations().lastLap().count() * 1e3);
    ImGui::Text("Last Transaction Update:");
    ImGui::Text(
        "    %.2f ms",
        interfaceStats.transactionUpdateTimer().lastLap().count() * 1e3);
    ImGui::Text("Constraints: %zu", interfaceStats.constraintsCount().count());
    ImGui::Text("Edit Vars: %zu", interfaceStats.editVariablesCount().count());
  }
  ImGui::End();
}

#endif

void StatisticsRenderer::render(Frame& frame,
                                CompositorStatistics& compositorStats) {
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
  RL_ASSERT(_StatisticsRenderer == nullptr);
  _StatisticsRenderer = this;

  ImGui::NewFrame();

  const double leftMargin = size.width - 260.0;
  double currentTopMargin = 20.0;

  ImGui::SetNextWindowSize(ImVec2(240, 120), ImGuiSetCond_FirstUseEver);
  ImGui::SetNextWindowPos(ImVec2(leftMargin, currentTopMargin),
                          ImGuiSetCond_Always);

  BuildStatsUI(compositorStats);

  ImGui::Render();

  _StatisticsRenderer = nullptr;
}

}  // namespace compositor
}  // namespace rl
