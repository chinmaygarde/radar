// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <ppapi/cpp/instance.h>
#include <ppapi/cpp/module.h>
#include <ppapi/cpp/var.h>

#include <nacl_io/nacl_io.h>
#include <Core/Core.h>

class RadarLoveInstance : public pp::Instance {
 public:
  explicit RadarLoveInstance(PP_Instance instance) : pp::Instance(instance) {}

  virtual ~RadarLoveInstance() {}

  virtual void HandleMessage(const pp::Var& var_message) {}
};

class RadarLoveModule : public pp::Module {
 public:
  RadarLoveModule() : pp::Module() {}

  virtual ~RadarLoveModule() {}

  virtual pp::Instance* CreateInstance(PP_Instance instance) {
    return new RadarLoveInstance(instance);
  }
};

namespace pp {
Module* CreateModule() {
  return new RadarLoveModule();
}
}  // namespace pp
