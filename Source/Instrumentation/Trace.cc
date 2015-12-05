// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Instrumentation/Trace.h>

#include <Core/ThreadLocal.h>
#include <unistd.h>

#include <sstream>

namespace rl {
namespace instrumentation {

RL_THREAD_LOCAL core::ThreadLocal CurrentThreadTrace([](uintptr_t value) {
  delete reinterpret_cast<ThreadTrace*>(value);
});

ThreadTrace& ThreadTrace::Current() {
  auto trace = reinterpret_cast<ThreadTrace*>(CurrentThreadTrace.get());
  if (trace != nullptr) {
    return *trace;
  }
  trace = new ThreadTrace();
  CurrentThreadTrace.set(reinterpret_cast<uintptr_t>(trace));
  return *trace;
}

static ProcessTrace CurrentProcessTrace;

ThreadTrace::ThreadTrace() : _threadID(std::this_thread::get_id()) {
  CurrentProcessTrace.addTrace(*this);
}

ThreadTrace::~ThreadTrace() {
  CurrentProcessTrace.removeTrace(*this);
}

void ThreadTrace::recordToStream(int pid, std::stringstream& stream) {
  std::lock_guard<std::mutex> lock(_eventsLock);

  auto count = _events.size();
  size_t current = 0;
  for (auto& event : _events) {
    event.recordToStream(pid, _threadID, stream);
    if (++current != count) {
      stream << ",";
    }
  }
}

size_t ThreadTrace::eventCount() const {
  return _events.size();
}

ProcessTrace::ProcessTrace() {
}

void ProcessTrace::addTrace(ThreadTrace& trace) {
  std::lock_guard<std::mutex> lock(_lock);
  _treadTraces.push_back(&trace);
}

void ProcessTrace::removeTrace(ThreadTrace& trace) {
  std::lock_guard<std::mutex> lock(_lock);
  _treadTraces.remove(&trace);
}

size_t ProcessTrace::traceCount() const {
  return _treadTraces.size();
}

ProcessTrace& ProcessTrace::Current() {
  return CurrentProcessTrace;
}

void ProcessTrace::recordToStream(std::stringstream& stream) {
  std::lock_guard<std::mutex> lock(_lock);

  auto pid = ::getpid();

  stream << "{\"traceEvents\":[";

  for (auto& trace : _treadTraces) {
    trace->recordToStream(pid, stream);
  }

  stream << "]}";
}

}  // namespace instrumentation
}  // namespace rl
