// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/CompositorStatisticsRenderer.h>
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
  virtual void onLinkSuccess() override {
    textureUniform = indexForUniform("Texture");
    projMtxUniform = indexForUniform("ProjMtx");

    positionAttribute = indexForAttribute("Position");
    uvAttribute = indexForAttribute("UV");
    colorAttribute = indexForAttribute("Color");
  }

  DISALLOW_COPY_AND_ASSIGN(CompositorStatisticsRendererProgram);
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
  const GLfloat width = ImGui::GetIO().DisplaySize.x;
  const GLfloat height = ImGui::GetIO().DisplaySize.y;

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
  renderer._program->use();

  glUniform1i(renderer._program->textureUniform, 0);
  glUniformMatrix4fv(renderer._program->projMtxUniform, 1, GL_FALSE,
                     &orthoProjection[0][0]);
  glBindVertexArray(renderer._vao);

  for (int n = 0; n < drawData->CmdListsCount; n++) {
    ImDrawList* cmdList = drawData->CmdLists[n];
    ImDrawIdx* idxBuffer = &cmdList->IdxBuffer.front();

    glBindBuffer(GL_ARRAY_BUFFER, renderer._vbo);

    int neededVtxSize = cmdList->VtxBuffer.size() * sizeof(ImDrawVert);

    if (renderer._vboSize < neededVtxSize) {
      // Grow our buffer if needed
      renderer._vboSize = neededVtxSize + 2000 * sizeof(ImDrawVert);
      glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)renderer._vboSize, NULL,
                   GL_STREAM_DRAW);
    }

    unsigned char* vtxData = (unsigned char*)glMapBufferRange(
        GL_ARRAY_BUFFER, 0, neededVtxSize,
        GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

    if (!vtxData) {
      continue;
    }

    memcpy(vtxData, &cmdList->VtxBuffer[0],
           cmdList->VtxBuffer.size() * sizeof(ImDrawVert));

    glUnmapBuffer(GL_ARRAY_BUFFER);

    for (const ImDrawCmd* pCmd = cmdList->CmdBuffer.begin();
         pCmd != cmdList->CmdBuffer.end(); pCmd++) {
      if (pCmd->UserCallback) {
        pCmd->UserCallback(cmdList, pCmd);
      } else {
        glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pCmd->TextureId);

        glScissor((int)(pCmd->ClipRect.x), (int)((height - pCmd->ClipRect.w)),
                  (int)((pCmd->ClipRect.z - pCmd->ClipRect.x)),
                  (int)((pCmd->ClipRect.w - pCmd->ClipRect.y)));

        glDrawElements(GL_TRIANGLES, (GLsizei)pCmd->ElemCount,
                       GL_UNSIGNED_SHORT, idxBuffer);
      }

      idxBuffer += pCmd->ElemCount;
    }
  }

  // Restore modified state
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_SCISSOR_TEST);
}

CompositorStatisticsRenderer::CompositorStatisticsRenderer()
    : _setupComplete(false),
      _program(nullptr),
      _vbo(GL_NONE),
      _vboSize(0),
      _vao(GL_NONE),
      _fontAtlas(GL_NONE) {
  auto& io = ImGui::GetIO();
  io.DisplaySize.x = 800;
  io.DisplaySize.y = 600;

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
  glDeleteVertexArrays(1, &_vao);
  glDeleteBuffers(1, &_vbo);
  glDeleteTextures(1, &_fontAtlas);
}

void CompositorStatisticsRenderer::performSetupIfNecessary() {
  if (_setupComplete) {
    return;
  }

  /*
   *  Create and initialize the shader program
   */
  _setupComplete = true;
  _program = rl::make_unique<CompositorStatisticsRendererProgram>();

  RL_GLAssert("There must be no errors prior to vertex buffer setup");

  /*
   *  Create and initialize the vertex buffers and vertex arrays
   */
  glGenVertexArrays(1, &_vao);
  glBindVertexArray(_vao);
  glGenBuffers(1, &_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, _vbo);
  glEnableVertexAttribArray(_program->positionAttribute);
  glEnableVertexAttribArray(_program->uvAttribute);
  glEnableVertexAttribArray(_program->colorAttribute);

#define OFFSETOF(TYPE, ELEMENT) ((size_t) & (((TYPE*)0)->ELEMENT))
  glVertexAttribPointer(_program->positionAttribute, 2, GL_FLOAT, GL_FALSE,
                        sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, pos));
  glVertexAttribPointer(_program->uvAttribute, 2, GL_FLOAT, GL_FALSE,
                        sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, uv));
  glVertexAttribPointer(_program->colorAttribute, 4, GL_UNSIGNED_BYTE, GL_TRUE,
                        sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, col));
#undef OFFSETOF

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  RL_GLAssert("There must be no error post vertex buffer setup");

  /*
   *  Create and initilize font atlases
   */
  RL_GLAssert("There must be no GL errors before setting up font atlases");

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

  RL_GLAssert("There must be no errors post compositor stat renderer setup");
}

void CompositorStatisticsRenderer::render() {
  performSetupIfNecessary();

  /*
   *  The framework does not allow for passing user pointers to the draw
   *  callback function. Since the stats renderer is a singleton anyway, we set
   *  a static global for the duration of the call.
   */
  _CompositorStatisticsRenderer = this;

  ImGui::NewFrame();

  ImGui::ShowTestWindow();

  ImGui::Render();

  _CompositorStatisticsRenderer = nullptr;
}

}  // namespace rl
