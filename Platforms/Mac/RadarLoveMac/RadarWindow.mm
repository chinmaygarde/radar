// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import "RadarWindow.h"

#include "Core/Utilities.h"
#include "Shell/Shell.h"
#include "Compositor/RenderSurface.h"
#include "Core/Message.h"

namespace rl {
class RenderSurfaceMac : public RenderSurface {
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
  DISALLOW_COPY_AND_ASSIGN(RenderSurfaceMac);
};
}

@interface RadarWindow ()<NSWindowDelegate>

@property(weak) IBOutlet NSOpenGLView* surface;

@end

@implementation RadarWindow {
  std::unique_ptr<rl::Shell> _shell;
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
  _shell = rl::Utils::make_unique<rl::Shell>(_renderSurface);
  _renderSurface->surfaceWasCreated();

  [self windowWasResized];
}

- (void)windowDidResize:(NSNotification*)notification {
  [self windowWasResized];
}

- (void)windowWasResized {
  const CGSize size = self.surface.bounds.size;
  _renderSurface->surfaceSizeUpdated(size.width, size.height);
}

- (void)mouseDown:(NSEvent*)theEvent {
}

- (void)mouseUp:(NSEvent*)theEvent {
}

- (void)mouseMoved:(NSEvent*)theEvent {
}

- (void)windowWillClose:(NSNotification*)notification {
  _shell->shutdown();
  _shell.reset();
}

- (void)dealloc {
  _renderSurface->surfaceWasDestroyed();
}

@end
