// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <ppapi/cpp/instance.h>
#include <ppapi/cpp/module.h>
#include <ppapi/cpp/var.h>
#include <ppapi/cpp/graphics_3d.h>
#include <ppapi/cpp/instance.h>
#include <ppapi/cpp/module.h>
#include <ppapi/cpp/var.h>
#include <ppapi/cpp/var_array.h>
#include <ppapi/lib/gl/gles2/gl2ext_ppapi.h>
#include <ppapi/utility/completion_callback_factory.h>
#include <ppapi/cpp/message_loop.h>

#include <nacl_io/nacl_io.h>
#include <Core/Core.h>
#include <Shell/Shell.h>
#include <Coordinator/RenderSurface.h>

#include "Sample.h"

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

class RadarLoveInstance : public pp::Instance {
 public:
  explicit RadarLoveInstance(PP_Instance instance) : pp::Instance(instance) {
    auto ppInstance = pp::Instance::pp_instance();
    auto ppInterface = pp::Module::Get()->get_browser_interface();
    RL_CHECK(nacl_io_init_ppapi(ppInstance, ppInterface));
  }

  virtual ~RadarLoveInstance() {
    _shell->shutdown();
    _shell.reset();

    RL_CHECK(nacl_io_uninit());
  }

  virtual void HandleMessage(const pp::Var& var_message) {}

  virtual void DidChangeView(const pp::View& view) {
    int32_t newWidth = view.GetRect().width() * view.GetDeviceScale();
    int32_t newHeight = view.GetRect().height() * view.GetDeviceScale();

    if (_context.is_null()) {
      auto result = initializeOpenGL(newWidth, newHeight);
      RL_ASSERT(result);
      initializeRadarLove();
    } else {
      auto result = _context.ResizeBuffers(newWidth, newHeight);
      RL_ASSERT(result >= 0);
    }

    resizeInterface(newWidth, newHeight);
  }

  void resizeInterface(int32_t width, int32_t height) {
    RL_ASSERT(_renderSurface != nullptr && _shell != nullptr);
    rl::geom::Size size(width, height);
    _renderSurface->surfaceSizeUpdated(size);
    _shell->interface().setSize(size);
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

  void initializeRadarLove() {
    _renderSurface = std::make_shared<RenderSurfaceNacl>(_context);
    _application = std::make_shared<sample::SampleApplication>();
    _shell =
        rl::core::make_unique<rl::shell::Shell>(_renderSurface, _application);
  }

 private:
  std::unique_ptr<rl::shell::Shell> _shell;
  std::shared_ptr<RenderSurfaceNacl> _renderSurface;
  std::shared_ptr<sample::SampleApplication> _application;

  pp::Graphics3D _context;

  RL_DISALLOW_COPY_AND_ASSIGN(RadarLoveInstance);
};

class RadarLoveModule : public pp::Module {
 public:
  RadarLoveModule() : pp::Module() {}

  virtual ~RadarLoveModule() {}

  virtual pp::Instance* CreateInstance(PP_Instance instance) {
    return new RadarLoveInstance(instance);
  }

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(RadarLoveModule);
};

namespace pp {
Module* CreateModule() {
  return new RadarLoveModule();
}
}  // namespace pp
