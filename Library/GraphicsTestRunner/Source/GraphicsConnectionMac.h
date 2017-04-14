/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Platform.h>
#include "GraphicsConnection.h"

#if RL_OS_MAC

#include <Core/ObjectiveCMacros.h>
#include <Core/ScopedNSObject.h>
#include <GLFoundation/GLFoundation.h>

RL_OBJC_CLASS(NSOpenGLContext);

namespace rl {
namespace testrunner {

class GraphicsConnectionMac : public GraphicsConnection {
 public:
  GraphicsConnectionMac(const geom::Size& size);

  ~GraphicsConnectionMac() override;

  bool isValid() const override;

  bool activate() override;

  bool deactivate() override;

 private:
  core::ScopedNSObject<NSOpenGLContext> _context;
  gl::GLFramebuffer _framebuffer;
  gl::GLTexture _texture;

  bool setupContext();

  bool setupFramebuffer(const geom::Size& size);

  bool makeCurrent();

  bool clearCurrent();

  RL_DISALLOW_COPY_AND_ASSIGN(GraphicsConnectionMac);
};

}  // namespace testrunner
}  // namespace rl

#endif  // RL_OS_MAC