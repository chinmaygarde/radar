// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_COORDINATOR_INTERFACECONTROLLER_H_
#define RADARLOVE_COORDINATOR_INTERFACECONTROLLER_H_

#include <Core/Core.h>
#include <Coordinator/PresentationGraph.h>
#include <Coordinator/Statistics.h>
#include <Coordinator/Channel.h>
#include <Event/TouchEvent.h>

namespace rl {
namespace coordinator {

class InterfaceController {
 public:
  InterfaceController();

  void scheduleChannel(core::EventLoop& loop, bool schedule);

  std::weak_ptr<Channel> channel() const;

  bool needsUpdate() const;

  bool updateInterface(const event::TouchEvent::PhaseMap& touchesIfAny);

  bool renderCurrentInterfaceState(Frame& frame);

 private:
  bool _needsUpdate;
  bool _isUpdating;
  std::shared_ptr<Channel> _channel;
  PresentationGraph _graph;
  Statistics _stats;

  void onChannelMessage(core::Messages messages);
  void setNeedsUpdate();

  bool applyPendingTouchEvents(const event::TouchEvent::PhaseMap& touches);
  bool applyAnimations();
  bool enforceConstraints();

  RL_DISALLOW_COPY_AND_ASSIGN(InterfaceController);
};

}  // namespace coordinator
}  // namespace rl

#endif  // RADARLOVE_COORDINATOR_INTERFACECONTROLLER_H_