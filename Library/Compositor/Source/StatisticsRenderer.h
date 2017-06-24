/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Compositor/Frame.h>
#include <Core/Macros.h>
#include <Core/Mutex.h>
#include <Core/Timing.h>
#include <Event/TouchEvent.h>
#include "StatisticsRendererProgram.h"

struct ImGuiIO;

namespace rl {
namespace compositor {

class StatisticsRenderer {
 public:
  static StatisticsRenderer* GetCurrent();

  StatisticsRenderer();

  ~StatisticsRenderer();

  bool applyTouches(const event::TouchEvent::PhaseMap& touches);

  void render(const Frame& frame);

  void beginSection(const char* section);

  void endSection();

  void displayValue(const char* format, va_list args);

  void getValue(const char* label, bool* current);

 private:
  core::Mutex _libraryMutex;
  bool _setupComplete;
  ImGuiIO& _io;
  std::map<event::TouchEvent::Identifier, geom::Point> _touches;
  std::unique_ptr<StatisticsRendererProgram> _program;
  unsigned int _vbo;
  unsigned int _fontAtlas;
  core::ClockPointSeconds _lastFrameTime;
  bool _framePending;

  static void drawLists(void* data);

  bool performRenderingSetupIfNecessary();

  static void SetCurrent(StatisticsRenderer*);

  bool ensureFrameStarted();

  RL_DISALLOW_COPY_AND_ASSIGN(StatisticsRenderer);
};

}  // namespace compositor
}  // namespace rl
