// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_COMPOSITOR_COMPOSITOR_
#define RADARLOVE_COMPOSITOR_COMPOSITOR_

#include <Core/Core.h>

#include <Geometry/Geometry.h>

#include <Compositor/RenderSurface.h>
#include <Compositor/ProgramCatalog.h>
#include <Compositor/EntityLease.h>
#include <Compositor/CompositorStatistics.h>
#include <Compositor/CompositorStatisticsRenderer.h>
#include <Compositor/PresentationGraph.h>
#include <Compositor/CompositorChannel.h>

#include <mutex>

namespace rl {
class Compositor : RenderSurfaceObserver {
 public:
  /**
   *  Create a compositor instance with the given render surface implementation.
   *  No operations on the surface are performed till the compositor is run
   *  however.
   */
  explicit Compositor(std::shared_ptr<RenderSurface> surface);

  ~Compositor();

#pragma mark - Managing Compositor Lifecycle

  /**
   *  Run the compositor on the current thread. This is a blocking operation and
   *  the compositor owns its loop.
   *
   *  @param readyLatch the latch triggered when the compositor has finished
   *         starting up and is about to wait for the first time on its loop.
   */
  void run(Latch& readyLatch);

  /**
   *  @return if the compositor is currently running
   */
  bool isRunning() const;

  /**
   *  Gracefully shutdown the compositor
   *
   *  @param shutdownLatch the latch triggered when the compositor is done
   *  shutting down.
   */
  void shutdown(Latch& shutdownLatch);

#pragma mark - Interface Lease Management

  std::weak_ptr<CompositorChannel> acquireChannel();

 private:
  std::shared_ptr<RenderSurface> _surface;
  EventLoop* _loop;
  std::mutex _lock;
  Size _surfaceSize;
  std::shared_ptr<ProgramCatalog> _programCatalog;
  std::shared_ptr<CompositorChannel> _interfaceChannel;
  PresentationGraph _graph;
  CompositorStatistics _stats;
  CompositorStatisticsRenderer _statsRenderer;
  std::shared_ptr<EventLoopSource> _animationsSource;

  void surfaceWasCreated() override;
  void surfaceSizeUpdated(const Size& size) override;
  void surfaceWasDestroyed() override;

  void startComposition();
  void commitCompositionSizeUpdate(const Size& size);
  void stopComposition();
  std::shared_ptr<ProgramCatalog> accessCatalog();
  void drawSingleFrame();
  bool applyTransactionMessages(Messages messages);
  void setupChannels();
  void teardownChannels();
  void manageInterfaceUpdates(bool schedule);
  void onAnimationsFlush();

  RL_DISALLOW_COPY_AND_ASSIGN(Compositor);
};
}  // namespace rl

#endif  // RADARLOVE_COMPOSITOR_COMPOSITOR_
