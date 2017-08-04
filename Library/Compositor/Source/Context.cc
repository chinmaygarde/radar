/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Compositor/Context.h>
#include <Core/Utilities.h>
#include <Geometry/PathBuilder.h>
#include <Geometry/Rect.h>
#include "ConsoleRenderer.h"
#include "ProgramCatalog.h"
#include "Vertices/BoxVertices.h"
#include "Vertices/StrokeVertices.h"

namespace rl {
namespace compositor {

Context::Context()
    : _beingUsed(false),
      _consoleRenderer(std::make_unique<ConsoleRenderer>()),
      _unitBoxVertices(
          std::make_unique<BoxVertices>(geom::Rect{0.0, 0.0, 1.0, 1.0})) {
  geom::PathBuilder builder;
  builder.addRect({0, 0, 100, 100});
  _unitBoxStrokeVertices = std::make_unique<StrokeVertices>(builder.path());
  auto sizeUpdated = _unitBoxStrokeVertices->setSize({1.0, 1.0});
  RL_ASSERT(sizeUpdated);
}

Context::~Context() = default;

CompositorStatistics& Context::statistics() {
  RL_ASSERT(_beingUsed);
  return _compositorStats;
}

ProgramCatalog& Context::programCatalog() {
  RL_ASSERT(_beingUsed);
  return *_programCatalog;
}

const BoxVertices& Context::unitBoxVertices() {
  RL_ASSERT(_beingUsed);
  return *_unitBoxVertices;
}

const StrokeVertices& Context::unitBoxStrokeVertices() {
  RL_ASSERT(_beingUsed);
  return *_unitBoxStrokeVertices;
}

bool Context::beginUsing() {
  if (_beingUsed || !_threadBinding.isBound()) {
    return false;
  }

  if (!_programCatalog) {
    _programCatalog = std::make_unique<ProgramCatalog>();
  }

  if (!_unitBoxVertices->prepare()) {
    return false;
  }

  if (!_unitBoxStrokeVertices->prepare()) {
    return false;
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
  return _consoleRenderer->applyTouches(touches);
}

void Context::renderConsole(const Frame& frame) {
  RL_ASSERT(_beingUsed);
  _consoleRenderer->render(frame);
}

bool Context::dispose() {
  if (_beingUsed) {
    return false;
  }

  _consoleRenderer = nullptr;
  _programCatalog = nullptr;
  _unitBoxVertices = nullptr;

  _threadBinding.unbind();

  return true;
}

}  // namespace compositor
}  // namespace rl
