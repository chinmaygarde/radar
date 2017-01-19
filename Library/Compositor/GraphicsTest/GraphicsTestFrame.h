// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_COMPOSITOR_GRAPHICSTESTFRAME_H_
#define RADAR_COMPOSITOR_GRAPHICSTESTFRAME_H_

#include <Compositor/Frame.h>
#include <Core/Macros.h>
#include <GraphicsTestRunner/GraphicsTest.h>

class GraphicsTestFrame : public GraphicsTest {
 public:
  GraphicsTestFrame();

  ~GraphicsTestFrame();

  rl::compositor::Frame& frame();

  bool snapshot();

 protected:
  void SetUp() override;

  void TearDown() override;

 private:
  rl::compositor::Context _context;
  rl::compositor::Frame _frame;
  size_t _snapshotsTaken;

  RL_DISALLOW_COPY_AND_ASSIGN(GraphicsTestFrame);
};

#endif  // RADAR_COMPOSITOR_GRAPHICSTESTFRAME_H_
