/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <jni.h>
#include <errno.h>

#include <EGL/egl.h>
#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#include <Core/Core.h>
#include <Shell/Shell.h>
#include <Coordinator/RenderSurface.h>
#include <Sample.h>

#define LOGI(...) \
  ((void)__android_log_print(ANDROID_LOG_INFO, "radarlove", __VA_ARGS__))
#define LOGW(...) \
  ((void)__android_log_print(ANDROID_LOG_WARN, "radarlove", __VA_ARGS__))

/**
 *  Shared state for our app.
 */
struct engine {
  struct android_app* app;

  EGLDisplay display;
  EGLSurface surface;
  EGLContext context;

  rl::shell::Shell* shell;
  rl::coordinator::RenderSurface* renderSurface;

  engine()
      : app(nullptr),
        display(EGL_NO_DISPLAY),
        surface(EGL_NO_SURFACE),
        context(EGL_NO_CONTEXT),
        width(0),
        height(0),
        shell(nullptr),
        renderSurface(nullptr) {}

  void setSize(int32_t w, int32_t h) {
    bool changed = w != width || h != height;

    width = w;
    height = h;

    if (changed) {
      rl::geom::Size size(w, h);

      if (renderSurface != nullptr) {
        renderSurface->surfaceSizeUpdated(size);
      }

      if (shell != nullptr) {
        shell->interface().setSize(size);
      }
    }
  }

  void attemptSizeUpdate() {
    EGLint newWidth = 0;
    EGLint newHeight = 0;

    EGLBoolean result = EGL_TRUE;

    result &= eglQuerySurface(display, surface, EGL_WIDTH, &newWidth);
    result &= eglQuerySurface(display, surface, EGL_HEIGHT, &newHeight);

    if (result == EGL_FALSE) {
      return;
    }

    setSize(newWidth, newHeight);
  }

  void handleMotionEvent(const AInputEvent* event) {
    if (AInputEvent_getType(event) != AINPUT_EVENT_TYPE_MOTION ||
        shell == nullptr) {
      return;
    }

    using Phase = rl::event::TouchEvent::Phase;
    int32_t action = AMotionEvent_getAction(event);
    Phase phase = Phase::Cancelled;
    switch (action & AMOTION_EVENT_ACTION_MASK) {
      case AMOTION_EVENT_ACTION_DOWN:
        phase = Phase::Began;
        break;
      case AMOTION_EVENT_ACTION_MOVE:
        phase = Phase::Moved;
        break;
      case AMOTION_EVENT_ACTION_UP:
        phase = Phase::Ended;
        break;
      default:
        return;
    }

    int32_t pointerIndex = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >>
                           AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
    auto x = AMotionEvent_getX(event, pointerIndex);
    auto y = AMotionEvent_getY(event, pointerIndex);
    auto pointerIdentifier = AMotionEvent_getPointerId(event, pointerIndex);

    shell->host().touchEventChannel().sendTouchEvents(
        {rl::event::TouchEvent{pointerIdentifier, {x, y}, phase}});
  }

 private:
  int32_t width;
  int32_t height;
};

/**
 *  Initialize an EGL context for the current display.
 */
static int engine_init_display(struct engine* engine) {
  /*
   *  Initialize OpenGL ES and EGL
   */

  /*
   *  Here specify the attributes of the desired configuration.
   *  Below, we select an EGLConfig with at least 8 bits per color
   *  component compatible with on-screen windows
   */
  const EGLint attribs[] = {EGL_SURFACE_TYPE,
                            EGL_WINDOW_BIT,
                            EGL_RENDERABLE_TYPE,
                            EGL_OPENGL_ES2_BIT,
                            EGL_BLUE_SIZE,
                            8,
                            EGL_GREEN_SIZE,
                            8,
                            EGL_RED_SIZE,
                            8,
                            EGL_NONE};

  const EGLint contextAttribs[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
  EGLint format;
  EGLint numConfigs;
  EGLConfig config;
  EGLSurface surface;
  EGLContext context;

  EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

  eglInitialize(display, 0, 0);

  /*
   *  Here, the application chooses the configuration it desires. In this
   *  sample, we have a very simplified selection process, where we pick
   *  the first EGLConfig that matches our criteria
   */
  eglChooseConfig(display, attribs, &config, 1, &numConfigs);

  /*
   *  EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
   *  guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
   *  As soon as we picked a EGLConfig, we can safely reconfigure the
   *  ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID.
   */
  eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

  ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);

  surface = eglCreateWindowSurface(display, config, engine->app->window, NULL);
  context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);

  if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
    LOGW("Unable to eglMakeCurrent");
    return -1;
  }

  engine->display = display;
  engine->context = context;
  engine->surface = surface;

  engine->attemptSizeUpdate();

  eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

  return 0;
}

/**
 *  Tear down the EGL context currently associated with the display.
 */
static void engine_term_display(struct engine* engine) {
  if (engine->display != EGL_NO_DISPLAY) {
    eglMakeCurrent(engine->display, EGL_NO_SURFACE, EGL_NO_SURFACE,
                   EGL_NO_CONTEXT);
    if (engine->context != EGL_NO_CONTEXT) {
      eglDestroyContext(engine->display, engine->context);
    }
    if (engine->surface != EGL_NO_SURFACE) {
      eglDestroySurface(engine->display, engine->surface);
    }
    eglTerminate(engine->display);
  }
  engine->display = EGL_NO_DISPLAY;
  engine->context = EGL_NO_CONTEXT;
  engine->surface = EGL_NO_SURFACE;
}

/**
 *  Process the next input event.
 */
static int32_t engine_handle_input(struct android_app* app,
                                   AInputEvent* event) {
  struct engine* engine = (struct engine*)app->userData;
  if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
    engine->handleMotionEvent(event);
    return 1;
  }
  return 0;
}

/**
 *  Process the next main command.
 */
static void engine_handle_cmd(struct android_app* app, int32_t cmd) {
  struct engine* engine = (struct engine*)app->userData;
  switch (cmd) {
    case APP_CMD_INIT_WINDOW:
      /*
       *  The window is being shown, get it ready.
       */
      if (engine->app->window != NULL) {
        engine_init_display(engine);
      }
      break;
    case APP_CMD_WINDOW_RESIZED:
      engine->attemptSizeUpdate();
      break;
    case APP_CMD_TERM_WINDOW:
      /*
       *  The window is being hidden or closed, clean it up.
       */
      engine_term_display(engine);
      break;
    case APP_CMD_GAINED_FOCUS:
      /*
       *  When our app gains focus, we start monitoring the accelerometer.
       */
      // TODO: Focus Radar
      break;
    case APP_CMD_LOST_FOCUS:
      // TODO: Unfocus Radar
      break;
  }
}

class RenderSurfaceAndroid : public rl::coordinator::RenderSurface {
 public:
  explicit RenderSurfaceAndroid(struct engine* engine)
      : RenderSurface(), _engine(engine) {
    RL_ASSERT(_engine != nullptr);
  }

  bool makeCurrent() {
    return isValid()
               ? eglMakeCurrent(_engine->display, _engine->surface,
                                _engine->surface, _engine->context) == EGL_TRUE
               : false;
  }

  bool present() {
    return isValid()
               ? eglSwapBuffers(_engine->display, _engine->surface) == EGL_TRUE
               : false;
  }

  bool isValid() const { return _engine->display != nullptr; }

 private:
  struct engine* _engine;

  RL_DISALLOW_COPY_AND_ASSIGN(RenderSurfaceAndroid);
};

/**
 *  This is the main entry point of a native application that is using
 *  android_native_app_glue.  It runs in its own thread, with its own
 *  event loop for receiving input events and doing other things.
 */
void android_main(struct android_app* state) {
  struct engine engine;

  /*
   *  Make sure glue isn't stripped.
   */
  app_dummy();

  memset(&engine, 0, sizeof(engine));
  state->userData = &engine;
  state->onAppCmd = engine_handle_cmd;
  state->onInputEvent = engine_handle_input;
  engine.app = state;

  auto renderSurface = std::make_shared<RenderSurfaceAndroid>(&engine);
  auto application = std::make_shared<sample::SampleApplication>();
  rl::shell::Shell shell(renderSurface, application);

  renderSurface->surfaceWasCreated();

  engine.shell = &shell;
  engine.renderSurface = renderSurface.get();

  /*
   *  Setup Platform Event Loop
   */
  bool running = true;
  while (running) {
    /*
     *  Read all pending events.
     */
    int ident;
    int events;
    struct android_poll_source* source;

    while ((ident = ALooper_pollAll(-1 /* indefinite */, NULL, &events,
                                    (void**)&source)) >= 0) {
      /*
       *  Process this event.
       */
      if (source != NULL) {
        source->process(state, source);
      }

      /*
       *  Check if we are exiting.
       */
      if (state->destroyRequested != 0) {
        engine_term_display(&engine);
        running = false;
        break;
      }
    }
  }

  renderSurface->surfaceWasDestroyed();
  shell.shutdown();
}
