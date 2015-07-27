// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_COMPOSITOR_COMPOSITOR__
#define __RADARLOVE_COMPOSITOR_COMPOSITOR__

#include "Core/Base.h"
#include "Core/Looper.h"
#include "Core/Lock.h"
#include "Core/Latch.h"

#include "Geometry/Size.h"

#include "Compositor/RenderSurface.h"
#include "Compositor/Layer.h"
#include "Compositor/ProgramCatalog.h"

namespace rl {
class Compositor : RenderSurfaceObserver {
 public:
  Compositor(std::shared_ptr<RenderSurface> surface);
  ~Compositor();

  void run(Latch& readyLatch);

  bool isRunning() { return _looper != nullptr; }

 private:
  std::shared_ptr<RenderSurface> _surface;
  Looper* _looper;
  Lock _lock;
  std::shared_ptr<LooperSource> _vsyncSource;
  Size _surfaceSize;
  Layer::Ref _rootLayer;
  std::shared_ptr<ProgramCatalog> _programCatalog;

  /*
   *  Render surface observer overrides
   */
  virtual void surfaceWasCreated() override;
  virtual void surfaceSizeUpdated(double width, double height) override;
  virtual void surfaceWasDestroyed() override;

  void startComposition();
  void commitCompositionSizeUpdate(double width, double height);
  void stopComposition();

  std::shared_ptr<ProgramCatalog> accessCatalog();

  void drawFrame();
  void onVsync();

  DISALLOW_COPY_AND_ASSIGN(Compositor);
};
}

#endif /* defined(__RADARLOVE_COMPOSITOR_COMPOSITOR__) */
