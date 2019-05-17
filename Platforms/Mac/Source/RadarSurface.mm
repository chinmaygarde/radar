/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "RadarSurface.h"
#include <Coordinator/RenderSurface.h>
#include <Samples/Sample.h>
#include <Shell/Shell.h>
#include <atomic>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

namespace rl {

class RenderSurfaceMac : public coordinator::RenderSurface {
 public:
  RenderSurfaceMac(NSOpenGLContext* context)
      : RenderSurface(), _context(context) {}

  virtual bool makeCurrent() override {
    [_context makeCurrentContext];
    return true;
  }

  virtual bool present() override {
    if (_rejectNonMainThreadPresents && ![NSThread isMainThread]) {
      return true;
    }

    [_context flushBuffer];
    return true;
  }

  virtual void accessWillBegin() override {
    /*
     *  OpenGL commands are about to begin. Lock the context so that window
     *  resizes don't interfere this frame.
     */
    auto error = CGLLockContext(_context.CGLContextObj);
    RL_ASSERT(error == kCGLNoError);
  }

  virtual void accessDidEnd() override {
    /*
     *  Frame access ended. Window resizes can now be serviced.
     */
    auto error = CGLUnlockContext(_context.CGLContextObj);
    RL_ASSERT(error == kCGLNoError);
  }

  void setRejectsNonMainThreadPresents(bool rejects) {
    _rejectNonMainThreadPresents = rejects;
  }

 private:
  NSOpenGLContext* _context;
  std::atomic_bool _rejectNonMainThreadPresents;

  RL_DISALLOW_COPY_AND_ASSIGN(RenderSurfaceMac);
};

}  // namespace rl

@implementation RadarSurface {
  std::shared_ptr<rl::RenderSurfaceMac> _renderSurface;
  std::unique_ptr<rl::shell::Shell> _shell;
  std::shared_ptr<rl::interface::InterfaceDelegate> _pendingApplicationLaunch;
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
   *  Create a managed interface and register it with the shell if we have a
   *  pending launch.
   */
  [self launchInterfaceDelegate:_pendingApplicationLaunch];
}

- (void)launchInterfaceDelegate:
    (std::shared_ptr<rl::interface::InterfaceDelegate>)delegate {
  if (delegate == nullptr) {
    _pendingApplicationLaunch = nullptr;
    /*
     *  Fallback to rendering the sample.
     */
    [self
        launchInterfaceDelegate:std::make_shared<sample::SampleApplication>()];
    return;
  }

  if (_shell == nullptr) {
    /*
     *  The shell is not yet ready. Store the delegate as pending launch.
     */
    _pendingApplicationLaunch = delegate;
    return;
  }

  auto interface = std::make_unique<rl::interface::Interface>(delegate);
  _shell->registerManagedInterface(std::move(interface));
  _pendingApplicationLaunch = nullptr;
}

- (void)reshape {
  CGSize size = self.bounds.size;

  _renderSurface->setRejectsNonMainThreadPresents(true);

  _shell->renderSurfaceDidUpdateSize({size.width, size.height});

  _shell->redrawCurrentFrameNow();

  _renderSurface->setRejectsNonMainThreadPresents(false);

  [super reshape];
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

  [super dealloc];
}

@end

#pragma clang diagnostic pop
