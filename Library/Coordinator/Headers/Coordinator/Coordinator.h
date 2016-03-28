// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_COORDINATOR_COORDINATOR_
#define RADARLOVE_COORDINATOR_COORDINATOR_

#include <Core/Core.h>

#include <Geometry/Geometry.h>

#include <Core/DebugTagGenerator.h>
#include <Coordinator/EntityLease.h>
#include <Coordinator/CoordinatorAcquisitionProtocol.h>
#include <Coordinator/InterfaceController.h>
#include <Coordinator/PresentationGraph.h>
#include <Coordinator/ProgramCatalog.h>
#include <Coordinator/CoordinatorStatistics.h>
#include <Coordinator/StatisticsRenderer.h>
#include <Event/TouchEventChannel.h>

#include <list>

namespace rl {
namespace coordinator {

class RenderSurface;

class Coordinator {
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
   *  @param onReady the callback that gets invoked when the coordinator is
   *                 ready
   */
  void run(std::function<void()> onReady = nullptr);

  /**
   *  @return if the coordinator is currently running
   */
  bool isRunning() const;

  /**
   *  Gracefully shutdown the coordinator
   *
   *  @param onShutdown the callback to be invoked on shutdown
   */
  void shutdown(std::function<void()> onShutdown = nullptr);

  void renderSurfaceWasSetup();

  void renderSurfaceSizeUpdated(const geom::Size& size);

  void renderSurfaceWasTornDown();

 private:
  std::shared_ptr<RenderSurface> _surface;
  core::EventLoop* _loop;
  geom::Size _surfaceSize;
  std::shared_ptr<ProgramCatalog> _programCatalog;
  core::DebugTagGenerator _interfaceTagGenerator;
  std::list<InterfaceController> _interfaceControllers;
  std::shared_ptr<core::EventLoopSource> _animationsSource;
  event::TouchEventChannel& _touchEventChannel;
  CoordinatorAcquisitionProtocol _coordinatorAcquisitionProtocol;
  CoordinatorStatistics _stats;
  StatisticsRenderer _statsRenderer;

  CoordinatorAcquisitionProtocol::VendorResult acquireFreshCoordinatorChannel();
  void startComposition();
  void commitCompositionSizeUpdate(const geom::Size& size);
  void stopComposition();
  std::shared_ptr<ProgramCatalog> accessCatalog();
  void setupOrTeardownChannels(bool setup);
  void scheduleInterfaceChannels(bool schedule);

  void renderFrame();

  void onDisplayLink();

  RL_DISALLOW_COPY_AND_ASSIGN(Coordinator);
};

}  // namespace coordinator
}  // namespace rl

#endif  // RADARLOVE_COORDINATOR_COORDINATOR_
