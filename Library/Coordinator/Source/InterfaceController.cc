// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Coordinator/InterfaceController.h>
#include <Core/TraceEvent.h>

namespace rl {
namespace coordinator {

InterfaceController::InterfaceController(const std::string& debugTag,
                                         const geom::Size& size)
    : _debugTag(debugTag),
      _localNS(),
      _channel(std::make_shared<core::Channel>()),
      _graph(_localNS, size, debugTag) {}

std::shared_ptr<core::Channel> InterfaceController::channel() const {
  return _channel;
}

const std::string& InterfaceController::debugTag() const {
  return _debugTag;
}

void InterfaceController::scheduleChannel(core::EventLoop& loop,
                                          bool schedule) {
  auto source = _channel->source();

  if (schedule) {
    namespace P = std::placeholders;
    _channel->setMessageCallback(
        std::bind(&InterfaceController::onChannelMessage, this, P::_1));
    loop.addSource(source);
  } else {
    _channel->setMessageCallback(nullptr);
    loop.removeSource(source);
  }
}

void InterfaceController::onChannelMessage(core::Message message) {
  RL_TRACE_AUTO(__function__);
  auto graphAccess = _graph.access();
  graphAccess.get().applyTransactions(message);
}

void InterfaceController::setSize(const geom::Size& size) {
  RL_TRACE_AUTO(__function__);
  auto graphAccess = _graph.access();
  graphAccess.get().updateSize(size);
}

bool InterfaceController::update(const event::TouchEvent::PhaseMap& touches) {
  RL_TRACE_AUTO(__function__);

  auto graphAccess = _graph.access();

  /*
   *  Step 1: Apply animations
   */
  bool animationsUpdated = applyAnimations(graphAccess);

  /*
   *  Step 2: Flush pending touches on the current state of the graph
   */
  bool touchesUpdated = applyPendingTouchEvents(graphAccess, touches);

  /*
   *  Step 3: Enforce constraints
   */
  bool constraintsEnforced = enforceConstraints(graphAccess);

  /*
   *  Step 4: Resolve any other visual updates not covered by any of the above
   *  cases.
   */
  bool hasVisualUpdates = graphAccess.get().resolveVisualUpdates();

  return (animationsUpdated || touchesUpdated || constraintsEnforced ||
          hasVisualUpdates);
}

bool InterfaceController::applyPendingTouchEvents(
    Graph::Access& access,
    const event::TouchEvent::PhaseMap& touches) {
  RL_TRACE_AUTO(__function__);
  return access.get().applyTouchMap(touches);
}

bool InterfaceController::applyAnimations(Graph::Access& access) {
  RL_TRACE_AUTO(__function__);
  return access.get().stepInterpolations();
}

bool InterfaceController::enforceConstraints(Graph::Access& access) {
  RL_TRACE_AUTO(__function__);
  return access.get().applyConstraints() > 0;
}

bool InterfaceController::render(compositor::Frame& frame) {
  RL_TRACE_AUTO(__function__);
  auto access = _graph.access();
  return access.get().render(frame);
}

}  // namespace coordinator
}  // namespace rl
