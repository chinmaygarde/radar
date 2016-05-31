// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_COORDINATOR_STATISTICSRENDERER_H_
#define RADARLOVE_COORDINATOR_STATISTICSRENDERER_H_

#include <Core/Core.h>
#include <Compositor/Frame.h>
#include <Compositor/CoordinatorStatistics.h>
#include <Compositor/InterfaceStatistics.h>

namespace rl {
namespace coordinator {

class StatisticsRendererProgram;
class StatisticsRenderer {
 public:
  explicit StatisticsRenderer();
  ~StatisticsRenderer();

  void render(Frame& frame,
              CoordinatorStatistics& coordinatorStats,
              const std::vector<std::reference_wrapper<InterfaceStatistics>>&
                  interfaceStats);

 private:
  bool _setupComplete;
  std::unique_ptr<StatisticsRendererProgram> _program;
  unsigned int _vbo;
  unsigned int _fontAtlas;

  void performSetupIfNecessary();
  void cleanup();
  static void drawLists(void* data);

  RL_DISALLOW_COPY_AND_ASSIGN(StatisticsRenderer);
};

}  // namespace coordinator
}  // namespace rl

#endif  // RADARLOVE_COORDINATOR_STATISTICSRENDERER_H_
