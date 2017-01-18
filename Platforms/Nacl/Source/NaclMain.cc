// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <ppapi/cpp/graphics_3d.h>
#include <ppapi/cpp/input_event.h>
#include <ppapi/cpp/instance.h>
#include <ppapi/cpp/instance.h>
#include <ppapi/cpp/message_loop.h>
#include <ppapi/cpp/module.h>
#include <ppapi/cpp/module.h>
#include <ppapi/cpp/var.h>
#include <ppapi/cpp/var.h>
#include <ppapi/cpp/var_array.h>
#include <ppapi/lib/gl/gles2/gl2ext_ppapi.h>
#include <ppapi/utility/completion_callback_factory.h>

#include <Coordinator/RenderSurface.h>
#include <Core/Core.h>
#include <Shell/Shell.h>
#include <nacl_io/nacl_io.h>

#include "Sample.h"

namespace rl {
namespace nacl {

class RenderSurfaceNacl : public rl::coordinator::RenderSurface {
 public:
  explicit RenderSurfaceNacl(pp::Graphics3D& context)
      : RenderSurface(), _context(context), _callbackFactory(this) {}

  bool makeCurrent() {
    glSetCurrentContextPPAPI(_context.pp_resource());
    return true;
  }

  bool present() {
    auto swapCallback =
        _callbackFactory.NewCallback(&RenderSurfaceNacl::swapOnMainThread);
    pp::MessageLoop::GetForMainThread().PostWork(swapCallback);
    return true;
  }

  void swapOnMainThread(int32_t) {
    auto callback =
        _callbackFactory.NewCallback(&RenderSurfaceNacl::didSwapBuffers);
    _context.SwapBuffers(callback);
  }

  void didSwapBuffers(int32_t) {}

 private:
  pp::Graphics3D _context;
  pp::CompletionCallbackFactory<RenderSurfaceNacl> _callbackFactory;

  RL_DISALLOW_COPY_AND_ASSIGN(RenderSurfaceNacl);
};

class RadarPlatformInstance : public pp::Instance {
 public:
  explicit RadarPlatformInstance(PP_Instance instance)
      : pp::Instance(instance), _tracingTouch(false) {
    auto ppInstance = pp::Instance::pp_instance();
    auto ppInterface = pp::Module::Get()->get_browser_interface();
    RL_CHECK(nacl_io_init_ppapi(ppInstance, ppInterface));

    auto result = RequestInputEvents(PP_INPUTEVENT_CLASS_MOUSE);
    RL_ASSERT(result == PP_OK);
  }

  ~RadarPlatformInstance() override {
    _shell->renderSurfaceWasTornDown();
    _shell->shutdown();
    _shell.reset();

    RL_CHECK(nacl_io_uninit());
  }

  bool HandleInputEvent(const pp::InputEvent& event) override {
    switch (event.GetType()) {
      /*
       *  TouchEvent::Phase::Began
       */
      case PP_INPUTEVENT_TYPE_MOUSEDOWN: {
        _tracingTouch = true;

        HandleEvent(pp::MouseInputEvent{event},
                    event::TouchEvent::Phase::Began);
      } break;
      /*
       *  TouchEvent::Phase::Ended
       */
      case PP_INPUTEVENT_TYPE_MOUSEUP: {
        _tracingTouch = false;

        HandleEvent(pp::MouseInputEvent{event},
                    event::TouchEvent::Phase::Ended);
      } break;
      /*
       *  TouchEvent::Phase::Moved
       */
      case PP_INPUTEVENT_TYPE_MOUSEMOVE: {
        if (!_tracingTouch) {
          break;
        }

        HandleEvent(pp::MouseInputEvent{event},
                    event::TouchEvent::Phase::Moved);
      } break;
      /*
       *  TouchEvent::Phase::Cancelled
       */
      case PP_INPUTEVENT_TYPE_MOUSELEAVE: {
        if (!_tracingTouch) {
          break;
        }

        HandleEvent(pp::MouseInputEvent{event},
                    event::TouchEvent::Phase::Cancelled);
      } break;
      default:
        break;
    }
    return true;
  }

  void HandleEvent(const pp::MouseInputEvent& event,
                   event::TouchEvent::Phase phase) {
    RL_ASSERT(!event.is_null());

    auto position = event.GetPosition();

    std::vector<event::TouchEvent> events;
    events.emplace_back(
        event::TouchEvent{0,
                          {static_cast<double>(position.x()),  //
                           static_cast<double>(position.y())},
                          phase});

    _shell->dispatchTouchEvents(events);
  }

  void Log(const pp::Var& value) { LogToConsole(PP_LOGLEVEL_LOG, value); }

  void HandleMessage(const pp::Var& var_message) override {}

  void DidChangeView(const pp::View& view) override {
    int32_t newWidth = view.GetRect().width() * view.GetDeviceScale();
    int32_t newHeight = view.GetRect().height() * view.GetDeviceScale();

    if (_context.is_null()) {
      auto result = initializeOpenGL(newWidth, newHeight);
      RL_ASSERT(result);
      initializeRadarPlatform();
    } else {
      auto result = _context.ResizeBuffers(newWidth, newHeight);
      RL_ASSERT(result >= 0);
    }

    resizeInterface(newWidth, newHeight);
  }

  void resizeInterface(int32_t width, int32_t height) {
    RL_ASSERT(_renderSurface != nullptr && _shell != nullptr);
    rl::geom::Size size(width, height);
    _shell->renderSurfaceDidUpdateSize(size);
  }

  bool initializeOpenGL(int32_t newWidth, int32_t newHeight) {
    if (!glInitializePPAPI(pp::Module::Get()->get_browser_interface())) {
      RL_LOG("Unable to initialize GL PPAPI!");
      return false;
    }

    const int32_t attrib_list[] = {PP_GRAPHICS3DATTRIB_ALPHA_SIZE, 8,
                                   PP_GRAPHICS3DATTRIB_DEPTH_SIZE, 24,
                                   PP_GRAPHICS3DATTRIB_WIDTH,      newWidth,
                                   PP_GRAPHICS3DATTRIB_HEIGHT,     newHeight,
                                   PP_GRAPHICS3DATTRIB_NONE};

    _context = pp::Graphics3D(this, attrib_list);
    if (!BindGraphics(_context)) {
      RL_LOG("Unable to bind 3d context!");
      _context = pp::Graphics3D();
      glSetCurrentContextPPAPI(0);
      return false;
    }

    return true;
  }

  void initializeRadarPlatform() {
    /*
     *  Create the render surface and initialize the shell.
     */
    _renderSurface = std::make_shared<RenderSurfaceNacl>(_context);
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

 private:
  std::unique_ptr<rl::shell::Shell> _shell;
  std::shared_ptr<RenderSurfaceNacl> _renderSurface;
  std::shared_ptr<sample::SampleApplication> _application;
  bool _tracingTouch;

  pp::Graphics3D _context;

  RL_DISALLOW_COPY_AND_ASSIGN(RadarPlatformInstance);
};

class RadarPlatformModule : public pp::Module {
 public:
  RadarPlatformModule() : pp::Module() {}

  virtual ~RadarPlatformModule() {}

  virtual pp::Instance* CreateInstance(PP_Instance instance) {
    return new RadarPlatformInstance(instance);
  }

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(RadarPlatformModule);
};

}  // namespace nacl
}  // namespace rl

namespace pp {
Module* CreateModule() {
  return new rl::nacl::RadarPlatformModule();
}
}  // namespace pp
