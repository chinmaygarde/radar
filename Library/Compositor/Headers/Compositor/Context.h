/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Compositor/CompositorStatistics.h>
#include <Compositor/ThreadBinding.h>
#include <Core/Macros.h>
#include <Event/TouchEvent.h>

namespace rl {
namespace compositor {

class ProgramCatalog;
class BoxVertices;
class ConsoleRenderer;
class Frame;
class StrokeVertices;

class Context {
 public:
  Context();

  ~Context();

  CompositorStatistics& statistics();

  ProgramCatalog& programCatalog();

  const BoxVertices& unitBoxVertices();

  const StrokeVertices& unitBoxStrokeVertices();

  RL_WARN_UNUSED_RESULT
  bool beginUsing();

  RL_WARN_UNUSED_RESULT
  bool endUsing();

  RL_WARN_UNUSED_RESULT
  bool dispose();

  void renderConsole(const Frame& frame);

  RL_WARN_UNUSED_RESULT
  bool applyTouchesToConsole(const event::TouchEvent::PhaseMap& touches);

 private:
  bool _beingUsed;
  ThreadBinding _threadBinding;
  CompositorStatistics _compositorStats;
  std::unique_ptr<ConsoleRenderer> _consoleRenderer;
  std::unique_ptr<ProgramCatalog> _programCatalog;
  std::unique_ptr<BoxVertices> _unitBoxVertices;
  std::unique_ptr<StrokeVertices> _unitBoxStrokeVertices;

  RL_DISALLOW_COPY_AND_ASSIGN(Context);
};

}  // namespace compositor
}  // namespace rl
