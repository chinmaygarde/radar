// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_COMPOSITOR_COMPOSITOR__
#define __RADARLOVE_COMPOSITOR_COMPOSITOR__

#include <Core/Core.h>

#include <Geometry/Geometry.h>

#include <Compositor/RenderSurface.h>
#include <Compositor/ProgramCatalog.h>
#include <Compositor/InterfaceLease.h>

#include <mutex>

namespace rl {
class Compositor : RenderSurfaceObserver {
 public:
  /**
   *  Create a compositor instance with the given render surface implementation.
   *  No operations on the surface are performed till the compositor is run
   *  however.
   */
  Compositor(std::shared_ptr<RenderSurface> surface);

  ~Compositor();

#pragma mark - Managing Compositor Lifecycle

  /**
   *  Run the compositor on the current thread. This is a blocking operation and
   *  the compositor owns its looper.
   *
   *  @param readyLatch the latch triggered when the compositor has finished
   *         starting up and is about to wait for the first time on its looper.
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

  InterfaceLease& acquireLease(size_t layer = 256);

 private:
  std::shared_ptr<RenderSurface> _surface;
  Looper* _looper;
  std::mutex _lock;
  Size _surfaceSize;
  std::shared_ptr<ProgramCatalog> _programCatalog;
  std::unique_ptr<InterfaceLease> _interfaceLease;

  virtual void surfaceWasCreated() override;
  virtual void surfaceSizeUpdated(const Size& size) override;
  virtual void surfaceWasDestroyed() override;
  void startComposition();
  void commitCompositionSizeUpdate(const Size& size);
  void stopComposition();
  std::shared_ptr<ProgramCatalog> accessCatalog();
  void drawSingleFrame();
  void onInterfaceDidUpdate();
  void setupChannels();
  void teardownChannels();
  void manageInterfaceUpdates(bool schedule);

  DISALLOW_COPY_AND_ASSIGN(Compositor);
};
}

#endif /* defined(__RADARLOVE_COMPOSITOR_COMPOSITOR__) */
