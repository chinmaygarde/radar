/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Compositor/Context.h>
#include <Coordinator/CoordinatorAcquisitionProtocol.h>
#include <Coordinator/InterfaceController.h>
#include <Coordinator/PresentationGraph.h>
#include <Core/DebugTagGenerator.h>
#include <Core/WorkQueue.h>
#include <Event/TouchEventChannel.h>
#include <Geometry/Size.h>
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
  Coordinator(std::shared_ptr<RenderSurface> surface,
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

  void redrawCurrentFrameNow();

 private:
  std::shared_ptr<RenderSurface> _surface;
  core::EventLoop* _loop;
  geom::Size _surfaceSize;
  compositor::Context _context;
  core::DebugTagGenerator _interfaceTagGenerator;
  core::WorkQueue _workQueue;
  core::Mutex _interfaceControllersMutex;
  std::list<InterfaceController> _interfaceControllers
      RL_GUARDED_BY(_interfaceControllersMutex);
  std::shared_ptr<core::EventLoopSource> _animationsSource;
  event::TouchEventChannel& _touchEventChannel;
  CoordinatorAcquisitionProtocol _coordinatorAcquisitionProtocol;
  bool _forceAnotherFrame;

  CoordinatorAcquisitionProtocol::VendorResult acquireFreshCoordinatorChannel();

  void setupOrTeardownChannels(bool setup);

  void scheduleInterfaceChannels(bool schedule)
      RL_REQUIRES(_interfaceControllersMutex);

  bool renderSingleFrame() RL_REQUIRES(_interfaceControllersMutex);

  void updateAndRenderInterfaceControllers(bool force);

  RL_DISALLOW_COPY_AND_ASSIGN(Coordinator);
};

}  // namespace coordinator
}  // namespace rl
