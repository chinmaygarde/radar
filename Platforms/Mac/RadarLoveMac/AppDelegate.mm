// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import "AppDelegate.h"
#include "Shell/Shell.h"
#include "Compositor/RenderSurface.h"

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
    return false;
  }

 private:
  NSOpenGLContext* _context;
  DISALLOW_COPY_AND_ASSIGN(RenderSurfaceMac);
};
}

@interface AppDelegate ()<NSWindowDelegate>

@property(weak) IBOutlet NSWindow* window;
@property(weak) IBOutlet NSOpenGLView* surface;

@end

@implementation AppDelegate {
  std::shared_ptr<rl::Shell> _shell;
  std::shared_ptr<rl::RenderSurfaceMac> _renderSurface;
}

- (void)applicationDidFinishLaunching:(NSNotification*)aNotification {
  _renderSurface =
      std::make_shared<rl::RenderSurfaceMac>(self.surface.openGLContext);
  _shell = std::make_shared<rl::Shell>(_renderSurface);
  _shell->attachHostOnCurrentThread();

  _renderSurface->surfaceWasCreated();

  self.window.delegate = self;

  [self windowWasResized];
}

- (void)windowDidResize:(NSNotification*)notification {
  [self windowWasResized];
}

- (void)windowWasResized {
  const CGSize size = self.surface.bounds.size;
  _renderSurface->surfaceSizeUpdated(size.width, size.height);
}

- (void)dealloc {
  _renderSurface->surfaceWasDestroyed();
}

@end
