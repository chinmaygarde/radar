// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_COMPOSITOR_STATISTICSRENDERER_H_
#define RADARLOVE_COMPOSITOR_STATISTICSRENDERER_H_

#include <Compositor/CompositorStatistics.h>
#include <Compositor/Frame.h>
#include <Compositor/InterfaceStatistics.h>
#include <Core/Core.h>

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

#endif  // RADARLOVE_COMPOSITOR_STATISTICSRENDERER_H_
