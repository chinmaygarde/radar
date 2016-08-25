// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_GRAPHICSTESTRUNNER_GRAPHICSTEST_H_
#define RADAR_GRAPHICSTESTRUNNER_GRAPHICSTEST_H_

#include <Core/Macros.h>
#include <Image/ImageResult.h>
#include <Geometry/Rect.h>

#include <gtest/gtest.h>

#include <memory>

namespace rl {
namespace testrunner {
class GraphicsConnection;
}
}

class GraphicsTest : public ::testing::Test {
 public:
  GraphicsTest();

  ~GraphicsTest();

  rl::image::ImageResult snapshot(const rl::geom::Size& viewport) const;

  rl::image::ImageResult snapshot(const rl::geom::Rect& viewport) const;

 protected:
  void SetUp() override;

  void TearDown() override;

 private:
  std::unique_ptr<rl::testrunner::GraphicsConnection> _connection;

  RL_DISALLOW_COPY_AND_ASSIGN(GraphicsTest);
};

#endif  // RADAR_GRAPHICSTESTRUNNER_GRAPHICSTEST_H_
