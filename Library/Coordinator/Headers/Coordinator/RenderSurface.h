/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <memory>

namespace rl {
namespace coordinator {

class RenderSurface {
 public:
  RenderSurface();

  virtual ~RenderSurface();

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

  virtual void accessWillBegin() = 0;

  virtual void accessDidEnd() = 0;
};

class ScopedRenderSurfaceAccess {
 public:
  ScopedRenderSurfaceAccess(RenderSurface& surface);

  ~ScopedRenderSurfaceAccess();

  bool acquired() const;

  bool finalized() const;

  void finalizeForPresentation();

 private:
  RenderSurface& _surface;
  bool _acquired;
  bool _finalized;

  RL_DISALLOW_COPY_AND_ASSIGN(ScopedRenderSurfaceAccess);
};

}  // namespace coordinator
}  // namespace rl
