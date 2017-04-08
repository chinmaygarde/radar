/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Coordinator/RenderSurface.h>
#include <Core/Core.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <Shell/Shell.h>
#include <assert.h>
#include <bcm_host.h>
#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <memory>

#include "Sample.h"

class PlatformEngine : public rl::coordinator::RenderSurface {
 public:
  PlatformEngine()
      : screen_width(0), screen_height(0), display(0), surface(0), context(0) {
    bcm_host_init();

    int32_t success = 0;
    EGLBoolean result = EGL_FALSE;
    EGLint num_config;

    DISPMANX_ELEMENT_HANDLE_T dispman_element;
    DISPMANX_DISPLAY_HANDLE_T dispman_display;
    DISPMANX_UPDATE_HANDLE_T dispman_update;
    VC_RECT_T dst_rect;
    VC_RECT_T src_rect;

    const EGLint attribute_list[] = {EGL_RED_SIZE,     8,
                                     EGL_GREEN_SIZE,   8,
                                     EGL_BLUE_SIZE,    8,
                                     EGL_ALPHA_SIZE,   8,
                                     EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                                     EGL_NONE};

    const EGLint context_attributes[] = {EGL_CONTEXT_CLIENT_VERSION, 2,
                                         EGL_NONE};
    EGLConfig config;

    /*
     *  Get an EGL display connection
     */
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    RL_ASSERT(display != EGL_NO_DISPLAY);

    /*
     *  Initialize the EGL display connection
     */
    result = eglInitialize(display, nullptr, nullptr);
    RL_ASSERT(EGL_FALSE != result);

    /*
     *  Get an appropriate EGL frame buffer configuration
     */
    result = eglChooseConfig(display, attribute_list, &config, 1, &num_config);
    RL_ASSERT(EGL_FALSE != result);

    /*
     *  Get an appropriate EGL frame buffer configuration
     */
    result = eglBindAPI(EGL_OPENGL_ES_API);
    RL_ASSERT(EGL_FALSE != result);

    /*
     *  Create an EGL rendering context
     */
    context =
        eglCreateContext(display, config, EGL_NO_CONTEXT, context_attributes);
    RL_ASSERT(context != EGL_NO_CONTEXT);

    /*
     *  Create an EGL window surface
     */
    success =
        graphics_get_display_size(0 /* LCD */, &screen_width, &screen_height);
    RL_ASSERT(success >= 0);

    dst_rect.x = 0;
    dst_rect.y = 0;
    dst_rect.width = screen_width;
    dst_rect.height = screen_height;

    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = screen_width << 16;
    src_rect.height = screen_height << 16;

    dispman_display = vc_dispmanx_display_open(0 /* LCD */);
    dispman_update = vc_dispmanx_update_start(0);

    dispman_element = vc_dispmanx_element_add(
        dispman_update, dispman_display, 0 /* Layer */, &dst_rect,
        0 /* Source */, &src_rect, DISPMANX_PROTECTION_NONE, 0 /* Alpha */,
        0 /* Clamp */, DISPMANX_NO_ROTATE /* Transform */);

    nativewindow.element = dispman_element;
    nativewindow.width = screen_width;
    nativewindow.height = screen_height;
    vc_dispmanx_update_submit_sync(dispman_update);

    surface = eglCreateWindowSurface(display, config, &nativewindow, nullptr);
    RL_ASSERT(surface != EGL_NO_SURFACE);

    attemptMouseConnection();
  }

  void attemptMouseConnection() {
    /*
     *  Connect to the mouse event descriptor
     */
    int mouseFD = open("/dev/input/mouse0", O_RDONLY | O_NONBLOCK);
    if (mouseFD <= 0) {
      RL_LOG("Did not detect or connect to a mouse...");
      return;
    }

    auto provider = [&mouseFD]() {
      return rl::core::EventLoopSource::Handles(mouseFD, -1);
    };

    auto collector = [](rl::core::EventLoopSource::Handles handles) {
      RL_CHECK(::close(static_cast<int>(handles.first)));
      RL_ASSERT(handles.second == -1);
    };

    auto reader = [&](rl::core::EventLoopSource::Handle readFD) {
      waitEvents(readFD);
    };

    auto source = std::make_shared<rl::core::EventLoopSource>(
        provider, collector, reader, nullptr, nullptr);

    rl::core::EventLoop::Current()->addSource(source);
  }

  bool makeCurrent() {
    return eglMakeCurrent(display, surface, surface, context) == EGL_TRUE;
  }

  bool present() { return eglSwapBuffers(display, surface) == EGL_TRUE; }

  int run(std::shared_ptr<rl::shell::Shell> shell) {
    surfaceWasCreated();

    /*
     *  Resize the shell
     */
    rl::geom::Size size(screen_width, screen_height);
    surfaceSizeUpdated(size);
    shell->interface().setSize(size);

    /*
     *  Start the main event loop
     */
    rl::core::EventLoop::Current()->loop();

    /*
     *  Teardown the shell
     */
    surfaceWasDestroyed();
    shell->shutdown();

    return EXIT_SUCCESS;
  }

 private:
  EGL_DISPMANX_WINDOW_T nativewindow;

  uint32_t screen_width;
  uint32_t screen_height;

  EGLDisplay display;
  EGLSurface surface;
  EGLContext context;

  void waitEvents(int descriptor) {
    const int width = screen_width, height = screen_height;
    int x = 800, y = 400;
    const int XSIGN = 1 << 4, YSIGN = 1 << 5;

    struct {
      char buttons, dx, dy;
    } payload;

    while (true) {
      int bytes = read(descriptor, &payload, sizeof(payload));

      if (bytes < sizeof(payload)) {
        dispatchMouseEvent(x, y);
        return;
      }

      if (payload.buttons & 8) {
        /*
         *  This bit should always be set
         */
        break;
      }

      /*
       *  Try to sync up again
       */
      read(descriptor, &payload, 1);
    }

    if (payload.buttons & 3) {
      return;
    }

    x += payload.dx;
    y += payload.dy;

    if (payload.buttons & XSIGN) {
      x -= 256;
    }

    if (payload.buttons & YSIGN) {
      y -= 256;
    }

    if (x < 0) {
      x = 0;
    }

    if (y < 0) {
      y = 0;
    }

    if (x > width) {
      x = width;
    }

    if (y > height) {
      y = height;
    }

    dispatchMouseEvent(x, y);
  }

  void dispatchMouseEvent(int x, int y) {
    RL_LOG("Mouse Event: %d x %d", x, y);
  }

  RL_DISALLOW_COPY_AND_ASSIGN(PlatformEngine);
};

int main() {
  auto engine = std::make_shared<PlatformEngine>();
  auto application = std::make_shared<sample::SampleApplication>();
  auto shell = std::make_shared<rl::shell::Shell>(engine, application);
  return engine->run(shell);
}
