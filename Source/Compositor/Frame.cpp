// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Compositor/Frame.h"

#include <cassert>

using namespace rl;

Frame::Frame(Matrix projectionMatrix,
             std::shared_ptr<const ProgramCatalog> catalog)
    : _projectionMatrix(projectionMatrix), _programCatalog(catalog) {
  assert(catalog != nullptr && "The program catalog must be valid");
}

void Frame::begin() {
}

void Frame::end() {
}
