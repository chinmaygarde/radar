// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Compositor/ProgramCatalog.h"
#include <cassert>

static const std::vector<std::string> DefaultUniforms = {"U_Model",
                                                         "U_View",
                                                         "U_Projection"};

static const std::string BasicVertexShader = R"--(

attribute vec4 position;

uniform mat4 U_Model;
uniform mat4 U_View;
uniform mat4 U_Projection;

void main() {
  gl_Position = U_Projection * U_View * U_Model * position;
}

)--";

static const std::string BasicFragmentShader = R"--(

#ifdef GL_ES
precision mediump float;
#endif

void main() {
  gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
}

)--";

using namespace rl;

ProgramCatalog::ProgramCatalog()
    : _prepared(false), _currentProgramType(ProgramTypeNone), _catalog() {
}

void ProgramCatalog::startUsing() {
  prepareIfNecessary();
  _currentProgramType = ProgramTypeNone;
}

void ProgramCatalog::stopUsing() {
  _currentProgramType = ProgramTypeNone;
}

bool ProgramCatalog::useProgramType(ProgramType type) {
  assert(_prepared &&
         "Catalog must be prepared and in use before using a program");

  if (_currentProgramType == type) {
    return true;
  }

  const auto& program = _catalog[type];

  if (program == nullptr) {
    return false;
  }

  if (program->use()) {
    _currentProgramType = type;
    return true;
  }

  return false;
}

void ProgramCatalog::prepareIfNecessary() {
  if (_prepared) {
    return;
  }

  _prepared = true;

  // Prepare Simple Primitive Program
  _catalog[ProgramTypeBasicPrimitve] = Utils::make_unique<Program>(
      DefaultUniforms, BasicVertexShader, BasicFragmentShader);
}

BasicPrimitiveProgram::BasicPrimitiveProgram()
    : Program::Program(DefaultUniforms,
                       BasicVertexShader,
                       BasicFragmentShader) {
}
