// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import "RadarSurface.h"

#include <Coordinator/RenderSurface.h>
#include <Core/Core.h>
#include <Shell/Shell.h>
#include "Sample.h"

namespace rl {

class RenderSurfaceMac : public coordinator::RenderSurface {
 public:
  RenderSurfaceMac(NSOpenGLContext* context)
      : RenderSurface(), _context(context) {}

  bool makeCurrent() {
    [_context makeCurrentContext];
    return true;
  }

  bool present() {
    [_context flushBuffer];
    return true;
  }

  void contextAccessWillBegin() {
    /*
     *  OpenGL commands are about to begin. Lock the context so that window
     *  resizes don't interfere this frame.
     */
    CGLLockContext(_context.CGLContextObj);
  }

  void contextAccessDidEnd() {
    /*
     *  Frame access ended. Window resizes can now be serviced.
     */
    CGLUnlockContext(_context.CGLContextObj);
  }

 private:
  NSOpenGLContext* _context;

  RL_DISALLOW_COPY_AND_ASSIGN(RenderSurfaceMac);
};

}  // namespace rl

@implementation RadarSurface {
  std::shared_ptr<rl::RenderSurfaceMac> _renderSurface;
  std::unique_ptr<rl::shell::Shell> _shell;
  std::shared_ptr<sample::SampleApplication> _application;
}

- (void)awakeFromNib {
  [super awakeFromNib];
  [self setupShell];
}

- (std::shared_ptr<rl::RenderSurfaceMac>)renderSurface {
  return _renderSurface;
}

- (void)setupShell {
  _renderSurface = std::make_shared<rl::RenderSurfaceMac>(self.openGLContext);

  /*
   *  Create the render surface and initialize the shell.
   */

  _shell = rl::shell::Shell::CreateWithCurrentThreadAsHost(_renderSurface);
  _shell->renderSurfaceWasSetup();

  /*
   *  Create a managed interface and register it with the shell.
   */
  _application = std::make_shared<sample::SampleApplication>();
  auto interface =
      rl::core::make_unique<rl::interface::Interface>(_application);
  _shell->registerManagedInterface(std::move(interface));
}

- (void)prepareOpenGL {
  [super prepareOpenGL];

  const GLint value = 1;
  [self.openGLContext setValues:&value forParameter:NSOpenGLCPSwapInterval];
}

- (void)reshape {
  _renderSurface->contextAccessWillBegin();

  CGSize size = self.bounds.size;
  _shell->renderSurfaceDidUpdateSize({size.width, size.height});

  _renderSurface->contextAccessDidEnd();
}

- (void)mouseDown:(NSEvent*)theEvent {
  [self dispatchEvent:theEvent phase:rl::event::TouchEvent::Phase::Began];
}

- (void)mouseUp:(NSEvent*)theEvent {
  [self dispatchEvent:theEvent phase:rl::event::TouchEvent::Phase::Ended];
}

- (void)mouseDragged:(nonnull NSEvent*)theEvent {
  [self dispatchEvent:theEvent phase:rl::event::TouchEvent::Phase::Moved];
}

- (BOOL)isFlipped {
  return YES;
}

- (BOOL)isOpaque {
  return YES;
}

- (void)dispatchEvent:(NSEvent*)event
                phase:(rl::event::TouchEvent::Phase)phase {
  NSPoint loc = [self convertPoint:event.locationInWindow fromView:nil];

  using Event = rl::event::TouchEvent;
  auto identifier = static_cast<uint64_t>(event.pointingDeviceID);

  std::vector<Event> events;
  events.emplace_back(Event{identifier, {loc.x, loc.y}, phase});
  _shell->dispatchTouchEvents(events);
}

- (void)teardownShell {
  _shell->renderSurfaceWasTornDown();
  _shell->shutdown();
  _shell.reset();
}

- (void)dealloc {
  [self teardownShell];
}

@end
