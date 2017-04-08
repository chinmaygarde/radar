/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "Config.h"

#if RL_OS_WINDOWS
#include <process.h>
#define getpid _getpid
#else  // RL_OS_WINDOWS
#include <unistd.h>
#endif  // RL_OS_WINDOWS

#include <Core/Thread.h>
#include <Core/ThreadLocal.h>
#include <Core/Trace.h>
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

  updateMetadata();
}

void ThreadTrace::UpdateMetadata() {
  auto trace = reinterpret_cast<ThreadTrace*>(CurrentThreadTrace.get());

  if (trace == nullptr) {
    /*
     *  Updating metadata does not itself initialize tracing.
     */
    return;
  }

  trace->updateMetadata();
}

void ThreadTrace::updateMetadata() {
  /*
   *  Record the thread name
   */
  recordEvent(TraceEvent::Type::Metadata, TraceEvent::Category::Default,
              TraceEventMetadataThreadName,
              TraceEvent::Arguments(
                  {{TraceEventMetadataNameKey, core::thread::GetName()}}));
}

ThreadTrace::~ThreadTrace() {
  CurrentProcessTrace.removeTrace(*this);
}

void ThreadTrace::recordToStream(int pid, std::stringstream& stream) {
  core::MutexLocker lock(_eventsLock);

  const auto count = _events.size();
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

ProcessTrace::ProcessTrace() {}

void ProcessTrace::addTrace(ThreadTrace& trace) {
  core::MutexLocker lock(_threadTracesMutex);
  _threadTraces.push_back(&trace);
}

void ProcessTrace::removeTrace(ThreadTrace& trace) {
  core::MutexLocker lock(_threadTracesMutex);
  _threadTraces.remove(&trace);
}

size_t ProcessTrace::traceCount() const {
  return _threadTraces.size();
}

ProcessTrace& ProcessTrace::Current() {
  return CurrentProcessTrace;
}

void ProcessTrace::recordToStream(std::stringstream& stream) {
  core::MutexLocker lock(_threadTracesMutex);

  auto pid = ::getpid();

  stream << "{\"traceEvents\":[";

  const auto count = _threadTraces.size();
  size_t current = 0;

  for (auto& trace : _threadTraces) {
    trace->recordToStream(pid, stream);
    if (++current != count) {
      stream << ",";
    }
  }

  stream << "]}";
}

}  // namespace instrumentation
}  // namespace rl
