// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_COORDINATOR_INTERFACECONTROLLER_H_
#define RADAR_COORDINATOR_INTERFACECONTROLLER_H_

#include <Compositor/FrontendPass.h>
#include <Compositor/InterfaceStatistics.h>
#include <Coordinator/PresentationGraph.h>
#include <Core/Channel.h>
#include <Core/Macros.h>
#include <Core/Mutexed.h>
#include <Event/TouchEvent.h>

namespace rl {
namespace coordinator {

class InterfaceController {
 public:
  InterfaceController(const std::string& debugTag, const geom::Size& size);

  void scheduleChannel(core::EventLoop& loop, bool schedule);

  std::shared_ptr<core::Channel> channel() const;

  void setSize(const geom::Size& size);

  bool update(const event::TouchEvent::PhaseMap& touchesIfAny);

  RL_WARN_UNUSED_RESULT
  compositor::FrontEndPass render();

  const std::string& debugTag() const;

 private:
  using Graph = core::Mutexed<PresentationGraph>;

  std::string _debugTag;
  core::Namespace _localNS;
  std::shared_ptr<core::Channel> _channel;
  Graph _graph;

  void onChannelMessage(core::Message message);
  void setNeedsUpdate();

  bool applyPendingTouchEvents(Graph::Access& access,
                               const event::TouchEvent::PhaseMap& touches);
  bool applyAnimations(Graph::Access& access);
  bool enforceConstraints(Graph::Access& access);

  RL_DISALLOW_COPY_AND_ASSIGN(InterfaceController);
};

}  // namespace coordinator
}  // namespace rl

#endif  // RADAR_COORDINATOR_INTERFACECONTROLLER_H_
