// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_GEOMETRY_RENDERSURFACE__
#define __RADARLOVE_GEOMETRY_RENDERSURFACE__

#include "Core/Base.h"
#include <memory>

namespace rl {

class RenderSurfaceObserver {
 public:
  virtual ~RenderSurfaceObserver() {}

  /**
   *  Invoked when the surface is first created.
   */
  virtual void surfaceWasCreated() = 0;

  /**
   *  Invoked when the surface size is updated
   *
   *  @param width  the new width
   *  @param height the new height
   */
  virtual void surfaceSizeUpdated(double width, double height) = 0;

  /**
   *  Invoked after the surface is destroyed
   */
  virtual void surfaceWasDestroyed() = 0;
};

class RenderSurface : RenderSurfaceObserver {
 public:
  RenderSurface() : _observer(nullptr) {}
  virtual ~RenderSurface() {}

  /**
   *  Make the current surface current for rendering into
   *
   *  @return if the operation was successful
   */
  virtual bool makeCurrent() = 0;

  /**
   *  Present the render surface on screen
   *
   *  @return if the operation was successful
   */
  virtual bool present() = 0;

  void setObserver(RenderSurfaceObserver* observer);

  /*
   *  Invoked by host on changes to underlying surface
   */
  virtual void surfaceWasCreated() override;
  virtual void surfaceSizeUpdated(double width, double height) override;
  virtual void surfaceWasDestroyed() override;

 private:
  RenderSurfaceObserver* _observer;
};
}

#endif /* defined(__RADARLOVE_GEOMETRY_RENDERSURFACE__) */
