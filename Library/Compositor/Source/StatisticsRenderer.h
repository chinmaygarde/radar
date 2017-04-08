/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Compositor/CompositorStatistics.h>
#include <Compositor/Frame.h>
#include <Compositor/InterfaceStatistics.h>
#include <Core/Macros.h>

namespace rl {
namespace compositor {

class StatisticsRendererProgram;
class StatisticsRenderer {
 public:
  StatisticsRenderer();

  ~StatisticsRenderer();

  void render(Frame& frame, CompositorStatistics& compositorStats);

 private:
  bool _setupComplete;

  std::unique_ptr<StatisticsRendererProgram> _program;
  unsigned int _vbo;
  unsigned int _fontAtlas;

  void performSetupIfNecessary();
  static void drawLists(void* data);

  RL_DISALLOW_COPY_AND_ASSIGN(StatisticsRenderer);
};

}  // namespace compositor
}  // namespace rl
