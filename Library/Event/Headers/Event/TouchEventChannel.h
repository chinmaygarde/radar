/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Channel.h>
#include <Event/TouchEvent.h>

namespace rl {
namespace event {

class TouchEventChannel : public core::Channel {
 public:
  TouchEventChannel();

  bool sendTouchEvents(const std::vector<TouchEvent>& touchEvents);

  TouchEvent::PhaseMap drainPendingTouches();

 private:
  std::vector<TouchEvent>& bufferForPhase(TouchEvent::Phase phase);

  RL_DISALLOW_COPY_AND_ASSIGN(TouchEventChannel);
};

}  // namespace event
}  // namespace rl
