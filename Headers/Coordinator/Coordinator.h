// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_COORDINATOR_COORDINATOR_
#define RADARLOVE_COORDINATOR_COORDINATOR_

#include <Core/Core.h>

#include <Geometry/Geometry.h>

#include <Coordinator/RenderSurface.h>
#include <Coordinator/ProgramCatalog.h>
#include <Coordinator/EntityLease.h>
#include <Coordinator/Statistics.h>
#include <Coordinator/StatisticsRenderer.h>
#include <Coordinator/PresentationGraph.h>
#include <Coordinator/Channel.h>
#include <Event/TouchEventChannel.h>

#include <mutex>

namespace rl {
namespace coordinator {

class Coordinator : RenderSurfaceObserver {
 public:
  /**
   *  Create a coordinator instance with the given render surface
   *  implementation. No operations on the surface are performed till the
   *  coordinator is run however.
   */
  explicit Coordinator(std::shared_ptr<RenderSurface> surface,
                       event::TouchEventChannel& touchEventChannel);

  ~Coordinator();

  /**
   *  Run the coordinator on the current thread. This is a blocking operation
   *  and the coordinator owns its loop.
   *
   *  @param readyLatch the latch triggered when the coordinator has finished
   *         starting up and is about to wait for the first time on its loop.
   */
  void run(core::Latch& readyLatch);

  /**
   *  @return if the coordinator is currently running
   */
  bool isRunning() const;

  /**
   *  Gracefully shutdown the coordinator
   *
   *  @param shutdownLatch the latch triggered when the coordinator is done
   *  shutting down.
   */
  void shutdown(core::Latch& shutdownLatch);

  std::weak_ptr<Channel> acquireChannel();

 private:
  std::shared_ptr<RenderSurface> _surface;
  core::EventLoop* _loop;
  std::mutex _lock;
  geom::Size _surfaceSize;
  std::shared_ptr<ProgramCatalog> _programCatalog;
  std::shared_ptr<Channel> _interfaceChannel;
  PresentationGraph _graph;
  Statistics _stats;
  StatisticsRenderer _statsRenderer;
  std::shared_ptr<core::EventLoopSource> _animationsSource;
  event::TouchEventChannel& _touchEventChannel;

  void surfaceWasCreated() override;
  void surfaceSizeUpdated(const geom::Size& size) override;
  void surfaceWasDestroyed() override;

  void startComposition();
  void commitCompositionSizeUpdate(const geom::Size& size);
  void stopComposition();
  std::shared_ptr<ProgramCatalog> accessCatalog();
  bool applyTransactionMessages(core::Messages messages);
  void setupChannels();
  void teardownChannels();
  void manageInterfaceUpdates(bool schedule);

  void prepareSingleFrame();
  void drawSingleFrame();

  void onInterfaceTransactionUpdate(core::Messages messages);
  void onAnimationsStep();
  void drainPendingTouches();

  RL_DISALLOW_COPY_AND_ASSIGN(Coordinator);
};

}  // namespace coordinator
}  // namespace rl

#endif  // RADARLOVE_COORDINATOR_COORDINATOR_
