// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <GraphicsTestRunner/GraphicsTest.h>

#include <Core/Utilities.h>
#include <Core/FileIOAdapter.h>
#include <Image/ImageEncoder.h>

#include "GraphicsConnection.h"

#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2.h>

GraphicsTest::GraphicsTest()
    : _connection(rl::core::make_unique<rl::testrunner::GraphicsConnection>()) {
}

GraphicsTest::~GraphicsTest() = default;

void GraphicsTest::SetUp() {
  auto setup = _connection->activate();
  RL_ASSERT_MSG(setup, "Must be able to setup the graphics connection");
}

void GraphicsTest::TearDown() {
  auto setup = _connection->deactivate();
  RL_ASSERT_MSG(setup, "Must be able to tear down the graphics connection");
}

rl::image::ImageResult GraphicsTest::snapshot(
    const rl::geom::Rect& viewport) const {
  if (viewport.size.width <= 0.0 || viewport.size.height <= 0.0) {
    return {};
  }

  size_t allocationSize = viewport.size.width * viewport.size.height * 4;

  rl::core::Allocation pixels;

  if (!pixels.resize(allocationSize)) {
    return {};
  }

  glReadPixels(viewport.origin.x, viewport.origin.y, viewport.size.width,
               viewport.size.height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

  if (glGetError() != GL_NO_ERROR) {
    return {};
  }

  return {
      {viewport.size.width, viewport.size.height},  // size
      rl::image::ImageResult::Components::RGBA,     // components
      std::move(pixels)                             // allocation
  };
}

bool GraphicsTest::snapshot(const rl::geom::Rect& viewport,
                            const rl::core::URI& destination) {
  /*
   *  Create the file to be written into.
   */
  rl::core::FileHandle writeHandle(
      destination, rl::core::FileHandle::AccessType::Write, true);

  if (!writeHandle.isValid()) {
    return false;
  }

  /*
   *  Snapshot the view.
   */
  auto imageResult = snapshot(viewport);

  if (!imageResult.wasSuccessful()) {
    return false;
  }

  /*
   *  Compress the snapshot to a PNG.
   */
  rl::image::ImageEncoder encoder(rl::image::ImageEncoder::Type::PNG,
                                  std::move(writeHandle));

  if (!encoder.isReady()) {
    return false;
  }

  /*
   *  Write the encoded snapshot to the file.
   */
  return encoder.encode(std::move(imageResult));
}
