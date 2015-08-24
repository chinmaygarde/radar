// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_COMPOSITOR_COMPOSITORSTATISTICSRENDERER_H__
#define __RADARLOVE_COMPOSITOR_COMPOSITORSTATISTICSRENDERER_H__

#include <Core/Core.h>
#include <Compositor/Program.h>
#include <Compositor/Frame.h>

namespace rl {
class CompositorStatisticsRendererProgram;
class CompositorStatisticsRenderer {
 public:
  CompositorStatisticsRenderer();
  ~CompositorStatisticsRenderer();

  void render(Frame& frame);

 private:
  bool _setupComplete;
  std::unique_ptr<CompositorStatisticsRendererProgram> _program;
  unsigned int _vbo;
  unsigned int _fontAtlas;

  void performSetupIfNecessary();
  void cleanup();
  static void drawLists(void* data);

  DISALLOW_COPY_AND_ASSIGN(CompositorStatisticsRenderer);
};

}  // namespace rl

#endif  // __RADARLOVE_COMPOSITOR_COMPOSITORSTATISTICSRENDERER_H__
