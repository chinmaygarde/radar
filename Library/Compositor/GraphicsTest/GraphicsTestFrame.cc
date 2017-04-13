/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "GraphicsTestFrame.h"

GraphicsTestFrame::GraphicsTestFrame()
    : _frame(kSurfaceSize, _context), _snapshotsTaken(0) {}

GraphicsTestFrame::~GraphicsTestFrame() {
  bool success = _context.dispose();
  RL_ASSERT(success);
}

bool GraphicsTestFrame::snapshot() {
  const auto& testInfo =
      ::testing::UnitTest::GetInstance()->current_test_info();

  std::stringstream stream;

  stream << testInfo->test_case_name() << "_" << testInfo->name();
  stream << "_" << ++_snapshotsTaken << ".png";

  return GraphicsTest::snapshot({_frame.size()}, rl::core::URI{stream.str()});
}

void GraphicsTestFrame::SetUp() {
  GraphicsTest::SetUp();

  ASSERT_TRUE(_frame.begin());
}

void GraphicsTestFrame::TearDown() {
  ASSERT_TRUE(_frame.end());

  GraphicsTest::TearDown();
}

rl::compositor::Frame& GraphicsTestFrame::frame() {
  return _frame;
}
