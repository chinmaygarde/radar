// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_INSTRUMENTATION_TRACE_H_
#define RADAR_INSTRUMENTATION_TRACE_H_

#include <Core/Macros.h>
#include <Core/Mutex.h>
#include <Core/TraceEvent.h>
#include <atomic>
#include <list>
#include <thread>

namespace rl {
namespace instrumentation {

class ThreadTrace {
 public:
  size_t eventCount() const;

  static void UpdateMetadata();

  ~ThreadTrace();

 private:
  std::thread::id _threadID;
  std::list<TraceEvent> _events;
  core::Mutex _eventsLock;

  friend class TraceEvent;
  friend class ProcessTrace;

  ThreadTrace();

  void recordToStream(int pid, std::stringstream& stream);

  template <class... T>
  void recordEvent(T&&... args) {
    core::MutexLocker lock(_eventsLock);
    _events.emplace_back(std::forward<T>(args)...);
  }

  static ThreadTrace& Current();

  void updateMetadata();

  RL_DISALLOW_COPY_AND_ASSIGN(ThreadTrace);
};

class ProcessTrace {
 public:
  ProcessTrace();

  size_t traceCount() const;

  static ProcessTrace& Current();

  void recordToStream(std::stringstream& stream);

 private:
  core::Mutex _threadTracesMutex;
  std::list<ThreadTrace*> _threadTraces;

  friend class ThreadTrace;

  void addTrace(ThreadTrace& trace);
  void removeTrace(ThreadTrace& trace);

  RL_DISALLOW_COPY_AND_ASSIGN(ProcessTrace);
};

}  // namespace instrumentation
}  // namespace rl

#endif  // RADAR_INSTRUMENTATION_TRACE_H_
