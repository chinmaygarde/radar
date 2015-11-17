﻿// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <string>

#include <memory>
#include <wrl.h>

// OpenGL ES includes
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

// EGL includes
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <EGL/eglplatform.h>

// ANGLE include for Windows Store
#include <angle_windowsstore.h>

namespace RadarLoveWindows {
ref class App sealed : public Windows::ApplicationModel::Core::IFrameworkView {
 public:
  App();

  // IFrameworkView Methods.
  virtual void Initialize(Windows::ApplicationModel::Core::CoreApplicationView ^
                          applicationView);
  virtual void SetWindow(Windows::UI::Core::CoreWindow ^ window);
  virtual void Load(Platform::String ^ entryPoint);
  virtual void Run();
  virtual void Uninitialize();

 private:
  void RecreateRenderer();

  // Application lifecycle event handlers.
  void OnActivated(Windows::ApplicationModel::Core::CoreApplicationView ^
                       applicationView,
                   Windows::ApplicationModel::Activation::IActivatedEventArgs ^
                       args);

  // Window event handlers.
  void OnVisibilityChanged(Windows::UI::Core::CoreWindow ^ sender,
                           Windows::UI::Core::VisibilityChangedEventArgs ^
                               args);
  void OnWindowClosed(Windows::UI::Core::CoreWindow ^ sender,
                      Windows::UI::Core::CoreWindowEventArgs ^ args);

  void InitializeEGL(Windows::UI::Core::CoreWindow ^ window);
  void CleanupEGL();

  bool mWindowClosed;
  bool mWindowVisible;

  EGLDisplay mEglDisplay;
  EGLContext mEglContext;
  EGLSurface mEglSurface;
};
}
