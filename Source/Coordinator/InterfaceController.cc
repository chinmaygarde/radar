// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Coordinator/InterfaceController.h>
#include <Instrumentation/TraceEvent.h>

namespace rl {
namespace coordinator {

class ScopedUpdate {
 public:
  ScopedUpdate(bool& flag) : _flag(flag) {
    RL_ASSERT_MSG(flag == false, "Cannot start a nested interface update");
    flag = true;
  }

  ~ScopedUpdate() { _flag = false; }

 private:
  bool& _flag;

  RL_DISALLOW_COPY_AND_ASSIGN(ScopedUpdate);
};

InterfaceController::InterfaceController(const std::string& debugTag)
    : _debugTag(debugTag),
      _localNS(),
      _needsUpdate(false),
      _isUpdating(false),
      _channel(std::make_shared<core::Channel>()),
      _graph(_localNS) {}

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
  RL_TRACE_AUTO("OnTransactionReceived");

  instrumentation::AutoStopwatchLap lap(_stats.transactionUpdateTimer());

  if (_graph.applyTransactions(message)) {
    setNeedsUpdate();
  }
}

bool InterfaceController::needsUpdate() const {
  return _needsUpdate;
}

void InterfaceController::setNeedsUpdate() {
  // WIP: This needs to be smarter about scheduling update requests
  _needsUpdate = true;
}

bool InterfaceController::updateInterface(
    const event::TouchEvent::PhaseMap& touches) {
  RL_TRACE_AUTO("UpdateInterface");

  if (!_needsUpdate && touches.size() == 0) {
    /*
     *  The interface was asked to update but there is no pending service call.
     */
    return false;
  }

  ScopedUpdate update(_isUpdating);

  /*
   *  Step 1: Apply animations
   */
  bool animationsUpdated = applyAnimations();

  /*
   *  Step 2: Flush pending touches on the current state of the graph
   */
  applyPendingTouchEvents(touches);

  /*
   *  Step 3: Enforce constraints
   */
  bool constraintsEnforced = enforceConstraints();

  _needsUpdate = animationsUpdated;

  return animationsUpdated || constraintsEnforced;
}

void InterfaceController::applyPendingTouchEvents(
    const event::TouchEvent::PhaseMap& touches) {
  RL_TRACE_AUTO("ApplyPendingTouches");

  RL_ASSERT_MSG(_isUpdating,
                "Can only apply touch updates within an update phase");

  if (touches.size() != 0) {
    _graph.applyTouchMap(touches);
  }
}

bool InterfaceController::applyAnimations() {
  RL_TRACE_AUTO("ApplyAnimations");

  RL_ASSERT_MSG(
      _isUpdating,
      "Can only apply animation interpolations within an update phase");

  const auto count =
      _graph.animationDirector().stepInterpolations(_stats.interpolations());

  _stats.interpolationsCount().reset(count);

  return count > 0;
}

bool InterfaceController::enforceConstraints() {
  RL_TRACE_AUTO("EnforceConstraints");

  RL_ASSERT_MSG(_isUpdating,
                "Can only enforce constraints within an update phase");

  return _graph.applyConstraints() == layout::Solver::FlushResult::Updated;
}

bool InterfaceController::renderCurrentInterfaceState(Frame& frame) {
  RL_TRACE_AUTO("RenderInterfaceState");

  RL_ASSERT_MSG(!_isUpdating,
                "Must not render in the middle of an interface update");

  return _graph.render(frame);
}

}  // namespace coordinator
}  // namespace rl
