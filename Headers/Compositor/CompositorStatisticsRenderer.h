// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_COMPOSITOR_COMPOSITORSTATISTICSRENDERER_H_
#define RADARLOVE_COMPOSITOR_COMPOSITORSTATISTICSRENDERER_H_

#include <Core/Core.h>
#include <Compositor/Frame.h>
#include <Compositor/CompositorStatistics.h>

namespace rl {
class CompositorStatisticsRendererProgram;
class CompositorStatisticsRenderer {
 public:
  explicit CompositorStatisticsRenderer();
  ~CompositorStatisticsRenderer();

  void render(CompositorStatistics& stats, Frame& frame);

 private:
  bool _setupComplete;
  std::unique_ptr<CompositorStatisticsRendererProgram> _program;
  unsigned int _vbo;
  unsigned int _fontAtlas;

  void performSetupIfNecessary();
  void buildStatsUI(CompositorStatistics& stats);
  void cleanup();
  static void drawLists(void* data);

  RL_DISALLOW_COPY_AND_ASSIGN(CompositorStatisticsRenderer);
};

}  // namespace rl

#endif  // RADARLOVE_COMPOSITOR_COMPOSITORSTATISTICSRENDERER_H_
