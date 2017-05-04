/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/FileIOAdapter.h>
#include <Core/Platform.h>
#include <Core/Utilities.h>
#include <GLFoundation/OpenGL.h>
#include <GraphicsTestRunner/GraphicsTest.h>
#include <Image/ImageEncoder.h>
#include <sstream>
#include "GraphicsConnection.h"

const rl::geom::Size GraphicsTest::kSurfaceSize = {320, 480};

GraphicsTest::GraphicsTest()
    : _connection(rl::testrunner::GraphicsConnection::Create(kSurfaceSize)) {
  RL_ASSERT(_connection->isValid());
}

GraphicsTest::~GraphicsTest() = default;

void GraphicsTest::SetUp() {
  auto setup = _connection->activate();
  ASSERT_TRUE(setup);
}

void GraphicsTest::TearDown() {
  auto teardown = _connection->deactivate();
  ASSERT_TRUE(teardown);
}

rl::image::ImageResult GraphicsTest::snapshot(
    const rl::geom::Rect& viewport) const {
  if (glGetError() != GL_NO_ERROR) {
    return {};
  }

  if (viewport.size.width <= 0.0 || viewport.size.height <= 0.0) {
    return {};
  }

  size_t allocationSize = viewport.size.width * viewport.size.height * 4;

  rl::core::Allocation pixels;

  if (!pixels.resize(allocationSize)) {
    return {};
  }

  glPixelStorei(GL_PACK_ALIGNMENT, 4);
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
