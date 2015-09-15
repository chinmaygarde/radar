// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <ppapi/cpp/instance.h>
#include <ppapi/cpp/module.h>
#include <ppapi/cpp/var.h>

#include <nacl_io/nacl_io.h>
#include <Core/Core.h>
#include <Shell/Shell.h>
#include <Compositor/RenderSurface.h>
#include "Sample.h"

class RenderSurfaceNacl : public rl::RenderSurface {
 public:
  explicit RenderSurfaceNacl() : RenderSurface() {}

  bool makeCurrent() { return false; }

  bool present() { return false; }

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(RenderSurfaceNacl);
};

class RadarLoveInstance : public pp::Instance {
 public:
  explicit RadarLoveInstance(PP_Instance instance) : pp::Instance(instance) {
    auto ppInstance = pp::Instance::pp_instance();
    auto ppInterface = pp::Module::Get()->get_browser_interface();
    RL_CHECK(nacl_io_init_ppapi(ppInstance, ppInterface));

    _renderSurface = std::make_shared<RenderSurfaceNacl>();
    _application = std::make_shared<sample::SampleApplication>();
    _shell = rl::make_unique<rl::Shell>(_renderSurface, _application);
  }

  virtual ~RadarLoveInstance() {
    _shell->shutdown();
    _shell.reset();
    RL_CHECK(nacl_io_uninit());
  }

  virtual void HandleMessage(const pp::Var& var_message) {}

 private:
  std::unique_ptr<rl::Shell> _shell;
  std::shared_ptr<RenderSurfaceNacl> _renderSurface;
  std::shared_ptr<sample::SampleApplication> _application;

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
