/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Core/URI.h>
#include <Geometry/Rect.h>
#include <Image/ImageResult.h>
#include <gtest/gtest.h>
#include <memory>

namespace rl {
namespace testrunner {
class GraphicsConnection;
}
}

class GraphicsTest : public ::testing::Test {
 public:
  static const rl::geom::Size kSurfaceSize;

  GraphicsTest();

  ~GraphicsTest();

  rl::image::ImageResult snapshot(const rl::geom::Rect& viewport) const;

  bool snapshot(const rl::geom::Rect& viewport,
                const rl::core::URI& destination);

 protected:
  void SetUp() override;

  void TearDown() override;

 private:
  std::unique_ptr<rl::testrunner::GraphicsConnection> _connection;

  RL_DISALLOW_COPY_AND_ASSIGN(GraphicsTest);
};
