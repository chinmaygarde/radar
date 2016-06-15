// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_COMPOSITOR_CONTEXT_H_
#define RADAR_COMPOSITOR_CONTEXT_H_

#include <Core/Macros.h>
#include <Compositor/CompositorStatistics.h>

namespace rl {
namespace compositor {

class ProgramCatalog;
class BoxVertices;
class StatisticsRenderer;
class Frame;

class Context {
 public:
  Context();

  ~Context();

  CompositorStatistics& statistics();

  ProgramCatalog& programCatalog();

  BoxVertices& unitBoxVertices();

  void beginUsing();

  void endUsing();

  void renderStatistics(Frame& frame);

  void dispose();

 private:
  bool _beingUsed;
  bool _disposed;
  CompositorStatistics _compositorStats;
  std::unique_ptr<StatisticsRenderer> _statsRenderer;
  std::unique_ptr<ProgramCatalog> _programCatalog;
  std::unique_ptr<BoxVertices> _unitBoxVertices;

  RL_DISALLOW_COPY_AND_ASSIGN(Context);
};

}  // namespace compositor
}  // namespace rl

#endif  // RADAR_COMPOSITOR_CONTEXT_H_
