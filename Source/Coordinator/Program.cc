// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if !RL_OS_WINDOWS

#include <Coordinator/Program.h>

#define RL_OPENGL_ALLOWED 1
#include <Coordinator/OpenGL.h>

#include <algorithm>

namespace rl {
namespace coordinator {

Program::Program(std::vector<std::string> attributes,
                 std::string vertexShader,
                 std::string fragmentShader)
    : _knownAttributes(attributes),
      _vertexShader(vertexShader),
      _fragmentShader(fragmentShader),
      _linkingComplete(false),
      _program(0) {
}

bool Program::use() {
  linkIfNecessary();

  RL_GLAssert("No errors before program use");
  glUseProgram(_program);

  return true;
}

static std::string Program_LogShaderInfo(GLuint shader) {
  GLint logSize = 0;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);

  if (logSize == 0) {
    return "";
  }

  auto log = reinterpret_cast<char*>(calloc(logSize, sizeof(char)));
  glGetShaderInfoLog(shader, logSize, &logSize, log);
  auto string = std::string(log);
  free(log);

  return string;
}

void Program::linkIfNecessary() {
  if (_linkingComplete) {
    return;
  }

  RL_ASSERT_MSG(!_linkingComplete, "Shader setup must not already be complete");
  RL_GLAssert("There must be no errors prior to shader setup");

  _program = glCreateProgram();
  RL_ASSERT_MSG(_program != 0, "Must be able to create a program");

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  RL_ASSERT_MSG(vertexShader != 0, "Must be able to create a vertex shader");

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  RL_ASSERT_MSG(fragmentShader != 0,
                "Must be able to create a fragment shader");

  auto vertexString = _vertexShader.c_str();
  auto fragmentString = _fragmentShader.c_str();

  glShaderSource(vertexShader, 1, &vertexString, nullptr);
  glShaderSource(fragmentShader, 1, &fragmentString, nullptr);

  glCompileShader(vertexShader);
  glCompileShader(fragmentShader);

  GLint status = GL_FALSE;
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);

  if (status != GL_TRUE) {
    RL_LOG("Vertex Shader Compilation Failed:\n%s\n",
           Program_LogShaderInfo(vertexShader).c_str());
    RL_ASSERT_MSG(false, "Vertex shader compilation must be successful");
  }

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE) {
    RL_LOG("Fragment Shader Compilation Failed:\n%s\n",
           Program_LogShaderInfo(fragmentShader).c_str());
    RL_ASSERT_MSG(false, "Fragment shader compilation must be successful");
  }

  glAttachShader(_program, vertexShader);
  glAttachShader(_program, fragmentShader);

  /*
   *  Attribute bindings must take place *before* program linking
   */
  auto count = _knownAttributes.size();
  for (auto i = 0; i < count; i++) {
    glBindAttribLocation(_program, i, _knownAttributes[i].c_str());
  }

  RL_GLAssert("Attribute bindings must be successful");

  glLinkProgram(_program);

  glGetProgramiv(_program, GL_LINK_STATUS, &status);
  RL_ASSERT_MSG(status == GL_TRUE, "The program must be successfully linked");

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  RL_GLAssert("There must be no errors post shader setup");

  _linkingComplete = true;

  onLinkSuccess();

  RL_GLAssert("Subclasses must not introduce program errors");
}

unsigned int Program::indexForAttribute(const std::string& attribute) {
  auto found =
      std::find(_knownAttributes.cbegin(), _knownAttributes.cend(), attribute);
  RL_ASSERT_MSG(found != _knownAttributes.end(), "Unknown attribute queried");
  return static_cast<unsigned int>(
      std::distance(_knownAttributes.cbegin(), found));
}

unsigned int Program::indexForUniform(const std::string& uniform) {
  RL_ASSERT_MSG(_linkingComplete,
                "May only access uniforms after linking is complete");
  GLint result = glGetUniformLocation(_program, uniform.c_str());
  RL_ASSERT_MSG(result != -1, "The unform must be found in the program");
  return result;
}

void Program::onLinkSuccess() {
}

Program::~Program() {
  /*
   *  Values of 0 are ignored.
   */
  glDeleteProgram(_program);
}

}  // namespace coordinator
}  // namespace rl

#endif  // !RL_OS_WINDOWS
