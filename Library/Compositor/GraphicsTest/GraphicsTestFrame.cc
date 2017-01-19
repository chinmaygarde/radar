// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "GraphicsTestFrame.h"

GraphicsTestFrame::GraphicsTestFrame()
    : _frame({320, 480}, _context), _snapshotsTaken(0) {}

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
