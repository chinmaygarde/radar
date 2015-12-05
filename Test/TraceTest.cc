// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <gtest/gtest.h>
#include <Instrumentation/Trace.h>
#include <Instrumentation/TraceEvent.h>
#include <thread>
#include <sstream>

using Event = rl::instrumentation::TraceEvent;

TEST(TraceTest, SimpleSetup) {
  Event::Record(Event::Type::Counter, Event::Category::Default, "counter1");
  Event::Record(Event::Type::Counter, Event::Category::Default, "counter2");
  Event::Record(Event::Type::Counter, Event::Category::Default, "counter3");
  Event::Record(Event::Type::Counter, Event::Category::Default, "counter4");
  auto& trace = rl::instrumentation::ProcessTrace::Current();
  ASSERT_EQ(trace.traceCount(), 1);

  std::stringstream stream;
  trace.recordToStream(stream);
  ASSERT_GE(stream.str().size(), 0);
}
