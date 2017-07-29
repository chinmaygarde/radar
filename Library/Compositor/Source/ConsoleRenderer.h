/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Compositor/Frame.h>
#include <Core/Instrumentation.h>
#include <Core/Macros.h>
#include <Core/Mutex.h>
#include <Core/Timing.h>
#include <Event/TouchEvent.h>

struct ImGuiIO;

namespace rl {
namespace compositor {

class ConsoleRendererProgram;

class ConsoleRenderer {
 public:
  static ConsoleRenderer* GetCurrent();

  ConsoleRenderer();

  ~ConsoleRenderer();

  bool applyTouches(const event::TouchEvent::PhaseMap& touches);

  void render(const Frame& frame);

  void beginSection(const char* section);

  void endSection();

  void displayLabel(const char* format, va_list args);

  void displayValue(const char* label,
                    const instrumentation::Stopwatch& stopwatch);

  void getValue(const char* label, bool* current);

  void getRange(const char* label, float* value, float minimum, float maximum);

 private:
  core::Mutex _libraryMutex;
  bool _setupComplete;
  ImGuiIO& _io;
  std::map<event::TouchEvent::Identifier, geom::Point> _touches;
  std::unique_ptr<ConsoleRendererProgram> _program;
  unsigned int _vbo;
  unsigned int _fontAtlas;
  core::ClockPointSeconds _lastFrameTime;
  bool _framePending;

  static void drawLists(void* data);

  bool performRenderingSetupIfNecessary();

  static void SetCurrent(ConsoleRenderer*);

  bool ensureFrameStarted();

  RL_DISALLOW_COPY_AND_ASSIGN(ConsoleRenderer);
};

}  // namespace compositor
}  // namespace rl
