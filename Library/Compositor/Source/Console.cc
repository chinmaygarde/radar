/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <imgui/imgui.h>
#include "Console.h"
#include "ConsoleRenderer.h"

namespace rl {
namespace compositor {
namespace console {

void BeginSection(const char* section) {
  if (auto renderer = ConsoleRenderer::GetCurrent()) {
    renderer->beginSection(section);
  }
}

void EndSection() {
  if (auto renderer = ConsoleRenderer::GetCurrent()) {
    renderer->endSection();
  }
}

bool IsFirstInFrame(FirstInFrame& once) {
  int currentFrame = ImGui::GetFrameCount();

  if (once.frameCount == currentFrame) {
    return false;
  }

  once.frameCount = currentFrame;
  return true;
}

void DisplayLabel(const char* format, ...) {
  if (auto renderer = ConsoleRenderer::GetCurrent()) {
    va_list args;
    va_start(args, format);
    renderer->displayLabel(format, args);
    va_end(args);
  }
}

void DisplayValue(const char* label,
                  const instrumentation::Stopwatch& stopwatch) {
  if (auto renderer = ConsoleRenderer::GetCurrent()) {
    renderer->displayValue(label, stopwatch);
  }
}

void GetValue(const char* label, bool* current) {
  if (auto renderer = ConsoleRenderer::GetCurrent()) {
    renderer->getValue(label, current);
  }
}

}  // namespace console
}  // namespace compositor
}  // namespace rl
