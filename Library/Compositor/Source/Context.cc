// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/Context.h>
#include <Geometry/Rect.h>

#include "ProgramCatalog.h"
#include "BoxVertices.h"
#include "StatisticsRenderer.h"

namespace rl {
namespace compositor {

Context::Context()
    : _beingUsed(false),
      _statsRenderer(core::make_unique<StatisticsRenderer>()),
      _programCatalog(core::make_unique<ProgramCatalog>()),
      _unitBoxVertices(
          core::make_unique<BoxVertices>(geom::Rect{0.0, 0.0, 1.0, 1.0})) {}

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

void Context::beginUsing() {
  RL_ASSERT(!_beingUsed);
  _beingUsed = true;

  _compositorStats.start();
}

void Context::endUsing() {
  RL_ASSERT(_beingUsed);
  _beingUsed = false;

  _compositorStats.stop();
}

void Context::renderStatistics(Frame& frame) {
  RL_ASSERT(_beingUsed);
  _statsRenderer->render(frame, _compositorStats);
}

Context::~Context() {
  RL_ASSERT(!_beingUsed);
}

}  // namespace compositor
}  // namespace rl
