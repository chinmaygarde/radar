// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Coordinator/InterfaceController.h>

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

InterfaceController::InterfaceController()
    : _channel(std::make_shared<Channel>()), _needsUpdate(false) {
}

std::weak_ptr<Channel> InterfaceController::channel() const {
  return _channel;
}

void InterfaceController::scheduleChannel(core::EventLoop& loop,
                                          bool schedule) {
  auto source = _channel->source();

  if (schedule) {
    namespace P = std::placeholders;
    _channel->setMessagesReceivedCallback(
        std::bind(&InterfaceController::onChannelMessage, this, P::_1));
    loop.addSource(source);
  } else {
    _channel->setMessagesReceivedCallback(nullptr);
    loop.removeSource(source);
  }
}

void InterfaceController::onChannelMessage(core::Messages messages) {
  instrumentation::AutoStopwatchLap lap(_stats.transactionUpdateTimer());

  bool success = true;

  for (auto& message : messages) {
    success &= _graph.applyTransactions(message);
  }

  if (success) {
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
  if (!_needsUpdate && touches.size() == 0) {
    /*
     *  The interface was asked to update but there is no pending service call.
     */
    return false;
  }

  ScopedUpdate update(_isUpdating);

  /*
   *  Step 1: Flush pending touches on the current state of the graph
   */
  bool touchesUpdated = applyPendingTouchEvents(touches);

  /*
   *  Step 2: Apply animations
   */
  bool animationsUpdated = applyAnimations();

  /*
   *  Step 3: Enforce constraints
   */
  bool constraintsEnforced = enforceConstraints();

  _needsUpdate = animationsUpdated;

  return touchesUpdated || animationsUpdated || constraintsEnforced;
}

bool InterfaceController::applyPendingTouchEvents(
    const event::TouchEvent::PhaseMap& touches) {
  RL_ASSERT_MSG(_isUpdating,
                "Can only apply touch updates within an update phase");

  return _graph.applyTouchMap(touches) ==
         recognition::RecognitionEngine::Result::Updated;
}

bool InterfaceController::applyAnimations() {
  RL_ASSERT_MSG(
      _isUpdating,
      "Can only apply animation interpolations within an update phase");

  const auto count =
      _graph.animationDirector().stepInterpolations(_stats.interpolations());

  _stats.interpolationsCount().reset(count);

  return count > 0;
}

bool InterfaceController::enforceConstraints() {
  RL_ASSERT_MSG(_isUpdating,
                "Can only enforce constraints within an update phase");
  return false;
}

bool InterfaceController::renderCurrentInterfaceState(Frame& frame) {
  RL_ASSERT_MSG(!_isUpdating,
                "Must not render in the middle of an interface update");
  _graph.render(frame);
  return true;
}

}  // namespace coordinator
}  // namespace rl