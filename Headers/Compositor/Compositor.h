// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_COMPOSITOR_COMPOSITOR__
#define __RADARLOVE_COMPOSITOR_COMPOSITOR__

#include <Core/Core.h>

#include <Geometry/Size.h>

#include <Compositor/RenderSurface.h>
#include <Compositor/PresentationLayer.h>
#include <Compositor/ProgramCatalog.h>
#include <Compositor/PresentationGraph.h>

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

#pragma mark - Communicating with the Compositor

  /**
   *  The channel used by interfaces to submit patches to this compositor
   *
   *  @return the patch channel
   */
  std::weak_ptr<Channel> patchChannel();

 private:
  std::shared_ptr<RenderSurface> _surface;
  Looper* _looper;
  std::mutex _lock;
  std::shared_ptr<LooperSource> _vsyncSource;
  Size _surfaceSize;
  PresentationLayer::Ref _rootLayer;
  std::shared_ptr<ProgramCatalog> _programCatalog;
  /*
   *  TODO: Decide if the graph should own its own channel so that multiple
   *  interface can connect to this compositor via their own channel.
   */
  std::shared_ptr<Channel> _patchChannel;
  PresentationGraph _presentationGraph;

  virtual void surfaceWasCreated() override;
  virtual void surfaceSizeUpdated(const Size& size) override;
  virtual void surfaceWasDestroyed() override;
  void startComposition();
  void commitCompositionSizeUpdate(const Size& size);
  void stopComposition();
  std::shared_ptr<ProgramCatalog> accessCatalog();
  void drawFrame();
  void onVsync();
  void setupChannels();
  void teardownChannels();
  void onPatchMessages(Messages messages);

  DISALLOW_COPY_AND_ASSIGN(Compositor);
};
}

#endif /* defined(__RADARLOVE_COMPOSITOR_COMPOSITOR__) */
