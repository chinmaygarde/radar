// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Compositor/ProgramCatalog.h"
#include <cassert>

static const std::string BasicVertexShader = R"--(

attribute vec3 position;

uniform mat4 U_MVP;
uniform vec2 U_Size;

void main() {
  gl_Position = U_MVP * vec4(position.x * U_Size.x,
                             position.y * U_Size.y,
                             0.0, 1.0);
}

)--";

static const std::string BasicFragmentShader = R"--(

#ifdef GL_ES
precision mediump float;
#endif

uniform vec4 U_ContentColor;

void main() {
  gl_FragColor = U_ContentColor;
}

)--";

namespace rl {

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
    : Program::Program({"U_MVP", "U_Size", "U_ContentColor"},
                       BasicVertexShader,
                       BasicFragmentShader),
      _modelViewProjectionUniform(-1),
      _contentColorUniform(-1),
      _sizeUniform(-1) {
}

void BasicPrimitiveProgram::onLinkSuccess() {
  _modelViewProjectionUniform = indexForUniform("U_MVP");
  _contentColorUniform = indexForUniform("U_ContentColor");
  _sizeUniform = indexForUniform("U_Size");
}

}  // namespace rl
