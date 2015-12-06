// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <gtest/gtest.h>
#include <Instrumentation/Trace.h>
#include <Instrumentation/TraceEvent.h>
#include <thread>
#include <sstream>

using Event = rl::instrumentation::TraceEvent;

TEST(TraceTest, SimpleTraceCapture) {
  Event::MarkDurationBegin(Event::Category::Default, "trace");
  Event::MarkCounter(Event::Category::Default, "counter1", 400);
  Event::MarkCounter(Event::Category::Default, "counter2", 200);
  Event::MarkCounter(Event::Category::Default, "counter3", 22);
  Event::MarkCounter(Event::Category::Default, "counter4", 88);
  Event::MarkCounter(Event::Category::Default, "counter4", 666);
  Event::MarkCounter(Event::Category::Default, "counter4", 999);
  Event::MarkCounter(Event::Category::Default, "counter4", 990);
  Event::MarkCounter(Event::Category::Default, "counter4", 222);
  Event::MarkCounter(Event::Category::Default, "counter4", 2);
  Event::MarkDurationEnd(Event::Category::Default, "trace");

  auto& trace = rl::instrumentation::ProcessTrace::Current();
  ASSERT_EQ(trace.traceCount(), 1);

  std::stringstream stream;
  trace.recordToStream(stream);
  ASSERT_GE(stream.str().size(), 0);

  RL_LOG("%s", stream.str().c_str());
}
