/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/Macros.h>
#include <Core/Trace.h>
#include <Core/TraceEvent.h>
#include <TestRunner/TestRunner.h>
#include <sstream>
#include <thread>

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
  std::stringstream stream;
  trace.recordToStream(stream);
  ASSERT_GE(stream.str().size(), 0u);
}

TEST(TraceTest, TraceUsingMacros) {
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
  ASSERT_GE(stream.str().size(), 0u);
}
