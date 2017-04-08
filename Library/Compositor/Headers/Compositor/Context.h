/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

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
