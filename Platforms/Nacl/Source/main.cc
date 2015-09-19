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

#include <GLES2/gl2.h>  // Remove

#include <nacl_io/nacl_io.h>
#include <Core/Core.h>
#include <Shell/Shell.h>
#include <Compositor/RenderSurface.h>

#include "Sample.h"

class RenderSurfaceNacl : public rl::RenderSurface {
 public:
  explicit RenderSurfaceNacl(pp::Graphics3D& context)
      : RenderSurface(), _context(context) {}

  bool makeCurrent() { return true; }

  bool present() { return true; }

 private:
  pp::Graphics3D _context;

  RL_DISALLOW_COPY_AND_ASSIGN(RenderSurfaceNacl);
};

class RadarLoveInstance : public pp::Instance {
 public:
  explicit RadarLoveInstance(PP_Instance instance)
      : pp::Instance(instance), _callbackFactory(this) {
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
    int32_t new_width = view.GetRect().width() * view.GetDeviceScale();
    int32_t new_height = view.GetRect().height() * view.GetDeviceScale();

    if (_context.is_null()) {
      auto result = InitializeOpenGL(new_width, new_height);
      RL_ASSERT(result);
    } else {
      auto result = _context.ResizeBuffers(new_width, new_height);
      RL_ASSERT(result >= 0);
    }

    // TODO: Use new_width and new_height
    RL_LOG("New Size %d x %d", new_width, new_height);
    glViewport(0, 0, new_width, new_height);
    glClearColor(0.0, 0.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    _context.SwapBuffers(
        _callbackFactory.NewCallback(&RadarLoveInstance::didSwapBuffers));
  }

  void didSwapBuffers(int32_t) { RL_LOG("Finished Swapping Buffers"); }

  bool InitializeOpenGL(int32_t new_width, int32_t new_height) {
    if (!glInitializePPAPI(pp::Module::Get()->get_browser_interface())) {
      RL_LOG("Unable to initialize GL PPAPI!");
      return false;
    }

    const int32_t attrib_list[] = {PP_GRAPHICS3DATTRIB_ALPHA_SIZE, 8,
                                   PP_GRAPHICS3DATTRIB_DEPTH_SIZE, 24,
                                   PP_GRAPHICS3DATTRIB_WIDTH,      new_width,
                                   PP_GRAPHICS3DATTRIB_HEIGHT,     new_height,
                                   PP_GRAPHICS3DATTRIB_NONE};

    _context = pp::Graphics3D(this, attrib_list);
    if (!BindGraphics(_context)) {
      RL_LOG("Unable to bind 3d context!");
      _context = pp::Graphics3D();
      glSetCurrentContextPPAPI(0);
      return false;
    }

    glSetCurrentContextPPAPI(_context.pp_resource());
    return true;
  }

  void InitializeRadarLove() {
    _renderSurface = std::make_shared<RenderSurfaceNacl>(_context);
    _application = std::make_shared<sample::SampleApplication>();
    _shell = rl::make_unique<rl::Shell>(_renderSurface, _application);
  }

 private:
  std::unique_ptr<rl::Shell> _shell;
  std::shared_ptr<RenderSurfaceNacl> _renderSurface;
  std::shared_ptr<sample::SampleApplication> _application;

  pp::Graphics3D _context;
  pp::CompletionCallbackFactory<RadarLoveInstance> _callbackFactory;

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
