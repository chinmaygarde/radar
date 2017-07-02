/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "ConsoleRenderer.h"
#include <Core/Utilities.h>
#include <GLFoundation/GLFoundation.h>
#include <imgui/imgui.h>
#include "Console.h"
#include "Program/ConsoleRendererProgram.h"

namespace rl {
namespace compositor {

static ConsoleRenderer* GConsoleRenderer = nullptr;

ConsoleRenderer* ConsoleRenderer::GetCurrent() {
  if (GConsoleRenderer == nullptr) {
    return nullptr;
  }

  return GConsoleRenderer->ensureFrameStarted() ? GConsoleRenderer : nullptr;
}

void ConsoleRenderer::SetCurrent(ConsoleRenderer* renderer) {
  GConsoleRenderer = renderer;
}

ConsoleRenderer::ConsoleRenderer()
    : _setupComplete(false),
      _io(ImGui::GetIO()),
      _program(nullptr),
      _vbo(GL_NONE),
      _fontAtlas(GL_NONE),
      _framePending(false) {
  _io.UserData = this;
  _io.RenderDrawListsFn = reinterpret_cast<void (*)(ImDrawData * data)>(
      &ConsoleRenderer::drawLists);
  SetCurrent(this);
}

ConsoleRenderer::~ConsoleRenderer() {
  SetCurrent(nullptr);

  _io.UserData = nullptr;

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

bool ConsoleRenderer::performRenderingSetupIfNecessary() {
  if (_setupComplete) {
    return false;
  }

  _setupComplete = true;

  /*
   *  Create and initialize the shader program
   */
  _program = std::make_unique<ConsoleRendererProgram>();

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

  _io.Fonts->GetTexDataAsRGBA32(reinterpret_cast<unsigned char**>(&pixels),
                                &fontAtlasWidth, &fontAtlasHeight);

  RL_ASSERT(pixels != nullptr && fontAtlasWidth != 0 && fontAtlasHeight != 0);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fontAtlasWidth, fontAtlasHeight, 0,
               GL_RGBA, GL_UNSIGNED_BYTE, pixels);

  _io.Fonts->TexID = reinterpret_cast<void*>((intptr_t)_fontAtlas);
  _io.Fonts->ClearInputData();
  _io.Fonts->ClearTexData();

  RL_GLAssert("There must be no errors post stat renderer setup");

  return true;
}

bool ConsoleRenderer::applyTouches(const event::TouchEvent::PhaseMap& touches) {
  bool touchesUpdated = false;

  for (auto phaseTouches : touches) {
    for (auto touch : phaseTouches.second) {
      switch (touch.phase()) {
        case event::TouchEvent::Phase::Began:
        case event::TouchEvent::Phase::Moved:
          _touches[touch.identifier()] = touch.location();
          touchesUpdated = true;
          break;
        case event::TouchEvent::Phase::Ended:
        case event::TouchEvent::Phase::Cancelled:
          _touches.erase(touch.identifier());
          touchesUpdated = true;
          break;
      }
    }
  }

  if (!touchesUpdated) {
    return false;
  }

  _io.MouseDown[0] = false;

  /*
   *  Just grab the first touch for now.
   */
  for (const auto& identifierTouch : _touches) {
    _io.MousePos.x = identifierTouch.second.x;
    _io.MousePos.y = identifierTouch.second.y;
    _io.MouseDown[0] = true;
    break;
  }

  if (!ensureFrameStarted()) {
    return false;
  }

  return _io.WantCaptureMouse || _io.WantCaptureKeyboard;
}

void ConsoleRenderer::drawLists(void* data) {
  if (data == nullptr) {
    return;
  }

  ConsoleRenderer& renderer =
      *reinterpret_cast<ConsoleRenderer*>(ImGui::GetIO().UserData);

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

void ConsoleRenderer::render(const Frame& frame) {
  core::MutexLocker lock(_libraryMutex);
  auto& size = frame.size();

  /*
   *  Update the display size.
   */
  _io.DisplaySize.x = size.width;
  _io.DisplaySize.y = size.height;

  /*
   *  Update display time.
   */
  auto now = core::Clock::now();
  core::ClockDurationSeconds delta = now - _lastFrameTime;
  _io.DeltaTime = delta.count();
  _lastFrameTime = now;

  /*
   *  Perform one time rendering setup if necessary. This has to wait till we
   *  get at least one frame size.
   */
  performRenderingSetupIfNecessary();

  if (_framePending) {
    ImGui::End();
    ImGui::Render();
    _framePending = false;
  }
}

bool ConsoleRenderer::ensureFrameStarted() {
  if (_framePending) {
    return true;
  }

  if (!_setupComplete) {
    return false;
  }

  ImGui::NewFrame();
  ImGui::Begin("Coordinator");
  _framePending = true;
  return true;
}

void ConsoleRenderer::beginSection(const char* section) {
  ImGui::Begin(section);
}

void ConsoleRenderer::endSection() {
  ImGui::End();
}

void ConsoleRenderer::displayLabel(const char* format, va_list args) {
  ImGui::TextV(format, args);
}

void ConsoleRenderer::displayValue(
    const char* label,
    const instrumentation::Stopwatch& stopwatch) {
  static const auto stopwatchValuesGetter = [](void* stopwatch,
                                               int index) -> float {
    auto lapTimeSeconds =
        reinterpret_cast<instrumentation::Stopwatch*>(stopwatch)->lapDuration(
            index);
    /*
     *  Plot the values in milliseconds.
     */
    return lapTimeSeconds.count() * 1e3;
  };

  ImGui::PlotLines(label,                  // label
                   stopwatchValuesGetter,  // getter
                   const_cast<instrumentation::Stopwatch*>(&stopwatch),  // data
                   stopwatch.samples(),     // samples size
                   0,                       // samples offset
                   "Range: 0.0 - 16.6 ms",  // overlay text
                   0.0,                     // scale min
                   1000.0 / 60.0            // scale max
                   );
}

void ConsoleRenderer::getValue(const char* label, bool* current) {
  ImGui::Checkbox(label, current);
}

}  // namespace compositor
}  // namespace rl
