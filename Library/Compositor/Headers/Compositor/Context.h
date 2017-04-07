// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <Compositor/CompositorStatistics.h>
#include <Compositor/ThreadBinding.h>
#include <Core/Macros.h>

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

  RL_WARN_UNUSED_RESULT
  bool beginUsing();

  RL_WARN_UNUSED_RESULT
  bool endUsing();

  RL_WARN_UNUSED_RESULT
  bool dispose();

  void renderStatistics(Frame& frame);

 private:
  bool _beingUsed;
  ThreadBinding _threadBinding;
  CompositorStatistics _compositorStats;
  std::unique_ptr<StatisticsRenderer> _statsRenderer;
  std::unique_ptr<ProgramCatalog> _programCatalog;
  std::unique_ptr<BoxVertices> _unitBoxVertices;

  RL_DISALLOW_COPY_AND_ASSIGN(Context);
};

}  // namespace compositor
}  // namespace rl
