/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

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
