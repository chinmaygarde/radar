/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

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
