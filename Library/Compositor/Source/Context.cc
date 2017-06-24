/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Compositor/Context.h>
#include <Core/Utilities.h>
#include <Geometry/Rect.h>
#include "ProgramCatalog.h"
#include "StatisticsRenderer.h"
#include "Vertices/BoxVertices.h"

namespace rl {
namespace compositor {

Context::Context()
    : _beingUsed(false),
      _statsRenderer(std::make_unique<StatisticsRenderer>()),
      _unitBoxVertices(
          std::make_unique<BoxVertices>(geom::Rect{0.0, 0.0, 1.0, 1.0})) {}

Context::~Context() = default;

CompositorStatistics& Context::statistics() {
  RL_ASSERT(_beingUsed);
  return _compositorStats;
}

ProgramCatalog& Context::programCatalog() {
  RL_ASSERT(_beingUsed);
  return *_programCatalog;
}

BoxVertices& Context::unitBoxVertices() {
  RL_ASSERT(_beingUsed);
  return *_unitBoxVertices;
}

bool Context::beginUsing() {
  if (_beingUsed || !_threadBinding.isBound()) {
    return false;
  }

  if (!_programCatalog) {
    _programCatalog = std::make_unique<ProgramCatalog>();
  }

  _compositorStats.start();

  _beingUsed = true;

  return true;
}

bool Context::endUsing() {
  if (!_beingUsed || !_threadBinding.isBound()) {
    return false;
  }

  _beingUsed = false;

  _compositorStats.stop();

  return true;
}

bool Context::applyTouchesToConsole(
    const event::TouchEvent::PhaseMap& touches) {
  return _statsRenderer->applyTouches(touches);
}

void Context::renderStatistics(const Frame& frame) {
  RL_ASSERT(_beingUsed);
  _statsRenderer->render(frame);
}

bool Context::dispose() {
  if (_beingUsed) {
    return false;
  }

  _statsRenderer = nullptr;
  _programCatalog = nullptr;
  _unitBoxVertices = nullptr;

  _threadBinding.unbind();

  return true;
}

}  // namespace compositor
}  // namespace rl
