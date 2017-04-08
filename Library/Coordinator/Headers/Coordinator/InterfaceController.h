/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Compositor/FrontendPass.h>
#include <Compositor/InterfaceStatistics.h>
#include <Coordinator/PresentationGraph.h>
#include <Core/Channel.h>
#include <Core/Macros.h>
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
  std::string _debugTag;
  core::Namespace _localNS;
  std::shared_ptr<core::Channel> _channel;
  core::Mutex _graphMutex;
  PresentationGraph _graph RL_GUARDED_BY(_graphMutex);

  void onChannelMessage(core::Message message);
  void setNeedsUpdate();

  bool applyPendingTouchEvents(const event::TouchEvent::PhaseMap& touches)
      RL_REQUIRES(_graphMutex);
  bool applyAnimations() RL_REQUIRES(_graphMutex);
  bool enforceConstraints() RL_REQUIRES(_graphMutex);

  RL_DISALLOW_COPY_AND_ASSIGN(InterfaceController);
};

}  // namespace coordinator
}  // namespace rl
