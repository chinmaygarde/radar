// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import "RadarWindow.h"

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

 private:
  NSOpenGLContext* _context;
  RL_DISALLOW_COPY_AND_ASSIGN(RenderSurfaceMac);
};
}

static void SendEvent(rl::event::TouchEventChannel& channel,
                      NSView* view,
                      NSEvent* event,
                      rl::event::TouchEvent::Phase phase) {
  NSPoint loc = [view convertPoint:event.locationInWindow fromView:nil];
  loc.y = view.frame.size.height - loc.y;

  using Event = rl::event::TouchEvent;
  auto identifier = static_cast<uint64_t>(event.pointingDeviceID);

  std::vector<Event> events;
  events.emplace_back(Event(identifier, {loc.x, loc.y}, phase));
  channel.sendTouchEvents(events);
}

@interface RadarWindow ()<NSWindowDelegate>

@property(weak) IBOutlet NSOpenGLView* surface;

@end

@implementation RadarWindow {
  std::unique_ptr<rl::shell::Shell> _shell;
  std::shared_ptr<rl::RenderSurfaceMac> _renderSurface;
}

- (void)awakeFromNib {
  [super awakeFromNib];
  self.delegate = self;
  [self launchShell];
}

- (void)launchShell {
  _renderSurface =
      std::make_shared<rl::RenderSurfaceMac>(self.surface.openGLContext);
  _shell = rl::core::make_unique<rl::shell::Shell>(_renderSurface);
  _renderSurface->surfaceWasCreated();
  [self windowWasResized];
}

- (void)windowDidResize:(NSNotification*)notification {
  [self windowWasResized];
}

- (void)windowWasResized {
  const CGSize boundsSize = self.surface.bounds.size;
  rl::geom::Size size(boundsSize.width, boundsSize.height);
  _renderSurface->surfaceSizeUpdated(size);
}

- (void)mouseDown:(NSEvent*)theEvent {
  SendEvent(_shell->host().touchEventChannel(), _surface, theEvent,
            rl::event::TouchEvent::Phase::Began);
}

- (void)mouseUp:(NSEvent*)theEvent {
  SendEvent(_shell->host().touchEventChannel(), _surface, theEvent,
            rl::event::TouchEvent::Phase::Ended);
}

- (void)mouseDragged:(nonnull NSEvent*)theEvent {
  SendEvent(_shell->host().touchEventChannel(), _surface, theEvent,
            rl::event::TouchEvent::Phase::Moved);
}

- (void)windowWillClose:(NSNotification*)notification {
  _shell->shutdown();
  _shell.reset();
}

- (void)dealloc {
  _renderSurface->surfaceWasDestroyed();
}

@end
