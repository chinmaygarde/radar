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
    : _prepared(false), _current(None, nullptr), _catalog() {
}

void ProgramCatalog::startUsing() {
  prepareIfNecessary();

  stopUsing();
}

void ProgramCatalog::stopUsing() {
  _current = std::make_pair(None, nullptr);
}

ProgramCatalog::ProgramRef ProgramCatalog::useProgramType(Type type) {
  assert(_prepared &&
         "Catalog must be prepared and in use before using a program");

  if (_current.first == type) {
    return _current.second;
  }

  const auto& program = _catalog[type];

  assert(program && "A valid program must be found in the catalog");

  bool result = program->use();

  assert(result && "The selected program must be usable by the catalog");
  _current = std::make_pair(type, program.get());

  return _current.second;
}

void ProgramCatalog::prepareIfNecessary() {
  if (_prepared) {
    return;
  }

  _prepared = true;

  // Prepare Simple Primitive Program
  _catalog[BasicPrimitve] = Utils::make_unique<BasicPrimitiveProgram>();
}

BasicPrimitiveProgram::BasicPrimitiveProgram()
    : Program::Program(DefaultUniforms, BasicVertexShader, BasicFragmentShader),
      _modelUniform(0),
      _viewUniform(0),
      _projectionUniform(0) {
}

void BasicPrimitiveProgram::onLinkSuccess() {
  _modelUniform = indexForUniform("U_Model");
  _viewUniform = indexForUniform("U_View");
  _projectionUniform = indexForUniform("U_Projection");
}
