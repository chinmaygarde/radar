/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

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
  core::MutexLocker lock(_graphMutex);
  _graph.applyTransactions(message);
}

void InterfaceController::setSize(const geom::Size& size) {
  RL_TRACE_AUTO(__function__);
  core::MutexLocker lock(_graphMutex);
  _graph.updateSize(size);
}

bool InterfaceController::update(const event::TouchEvent::PhaseMap& touches) {
  RL_TRACE_AUTO(__function__);

  core::MutexLocker lock(_graphMutex);

  /*
   *  Step 1: Apply animations
   */
  bool animationsUpdated = applyAnimations();

  /*
   *  Step 2: Flush pending touches on the current state of the graph
   */
  bool touchesUpdated = applyPendingTouchEvents(touches);

  /*
   *  Step 3: Enforce constraints
   */
  bool constraintsEnforced = enforceConstraints();

  /*
   *  Step 4: Resolve any other visual updates not covered by any of the above
   *  cases.
   */
  bool hasVisualUpdates = _graph.resolveVisualUpdates();

  return (animationsUpdated || touchesUpdated || constraintsEnforced ||
          hasVisualUpdates);
}

bool InterfaceController::applyPendingTouchEvents(
    const event::TouchEvent::PhaseMap& touches) {
  RL_TRACE_AUTO(__function__);
  return _graph.applyTouchMap(touches);
}

bool InterfaceController::applyAnimations() {
  RL_TRACE_AUTO(__function__);
  return _graph.stepInterpolations();
}

bool InterfaceController::enforceConstraints() {
  RL_TRACE_AUTO(__function__);
  return _graph.applyConstraints() > 0;
}

compositor::FrontEndPass InterfaceController::render() {
  RL_TRACE_AUTO(__function__);
  compositor::FrontEndPass pass;
  core::MutexLocker lock(_graphMutex);
  _graph.render(pass);
  return pass;
}

}  // namespace coordinator
}  // namespace rl
