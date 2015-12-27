// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Macros.h>
#include <Instrumentation/Trace.h>
#include <Instrumentation/TraceEvent.h>
#include <gtest/gtest.h>
#include <sstream>
#include <thread>

using Event = rl::instrumentation::TraceEvent;

TEST(TraceTest, RL_KNOWN_SLOW(SimpleTraceCapture)) {
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
  std::stringstream stream;
  trace.recordToStream(stream);
  ASSERT_GE(stream.str().size(), 0);
}

TEST(TraceTest, RL_KNOWN_SLOW(TraceUsingMacros)) {
  {
    RL_TRACE_AUTO("auto");
    {
      RL_TRACE_AUTO("auto2");
      RL_TRACE_ASYNC_INSTANT("instant");
      RL_TRACE_BEGIN("something");
      RL_TRACE_END("something");
      RL_TRACE_ASYNC_END("instant");
      RL_TRACE_COUNT("count", 22);
      RL_TRACE_COUNT("count", 55);
      RL_TRACE_COUNT("count", 1);
      RL_TRACE_COUNT("count", 200);
    }
  }

  auto& trace = rl::instrumentation::ProcessTrace::Current();
  std::stringstream stream;
  trace.recordToStream(stream);
  ASSERT_GE(stream.str().size(), 0);
}
