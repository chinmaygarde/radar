// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <string.h>

#include <GLES2/gl2.h>

class SimpleRenderer {
 public:
  SimpleRenderer();
  ~SimpleRenderer();
  void Draw();
  void UpdateWindowSize(GLsizei width, GLsizei height);

 private:
  GLuint mProgram;
  GLsizei mWindowWidth;
  GLsizei mWindowHeight;

  GLint mPositionAttribLocation;
  GLint mColorAttribLocation;

  GLint mModelUniformLocation;
  GLint mViewUniformLocation;
  GLint mProjUniformLocation;

  GLuint mVertexPositionBuffer;
  GLuint mVertexColorBuffer;
  GLuint mIndexBuffer;

  int mDrawCount;
};
