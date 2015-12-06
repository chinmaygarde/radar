// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_INSTRUMENTATION_TRACEEVENT_H_
#define RADARLOVE_INSTRUMENTATION_TRACEEVENT_H_

#include <Core/Core.h>

#include <map>
#include <thread>

/*
 *  Tracing
 */
#define RL_TRACING_ENABLE 1

#if RL_TRACING_ENABLE

#define _RL_E rl::instrumentation::TraceEvent
#define _RL_C _RL_E::Category::Default

#define RL_TRACE_BEGIN(name) _RL_E::MarkDurationBegin(_RL_C, (name));
#define RL_TRACE_END(name) _RL_E::MarkDurationEnd(_RL_C, (name));
#define RL_TRACE_INSTANT(name) _RL_E::MarkInstant(_RL_C, (name));
#define RL_TRACE_ASYNC_START(name) _RL_E::MarkAsyncStart(_RL_C, (name));
#define RL_TRACE_ASYNC_INSTANT(name) _RL_E::MarkAsyncInstant(_RL_C, (name));
#define RL_TRACE_ASYNC_END(name) _RL_E::MarkAsyncEnd(_RL_C, (name));
#define RL_TRACE_COUNT(name, count) _RL_E::MarkCounter(_RL_C, (name), (count));

#define RL_TRACE_AUTO(name) \
  rl::instrumentation::AutoDurationEvent _rl_trace_##__LINE__(_RL_C, name);

#else  // RL_TRACING_ENABLE

#define RL_TRACE_BEGIN(name)
#define RL_TRACE_END(name)
#define RL_TRACE_INSTANT(name)
#define RL_TRACE_ASYNC_START(name)
#define RL_TRACE_ASYNC_INSTANT(name)
#define RL_TRACE_ASYNC_END(name)
#define RL_TRACE_COUNT(name, count)

#define RL_TRACE_AUTO(name)

#endif  // RL_TRACING_ENABLE

namespace rl {
namespace instrumentation {

class TraceEvent {
 public:
  using TraceClockPoint = core::ClockPointMicro;
  using TraceClockDuration = core::ClockDurationMicro;
  using Arguments = std::map<std::string, std::string>;

  enum class Type : char {
    DurationBegin = 'B',
    DurationEnd = 'E',
    Instant = 'i',
    Counter = 'C',
    AsyncStart = 'b',
    AsyncInstant = 'n',
    AsyncEnd = 'e',
  };

  enum class Category : uint8_t {
    Default,
  };

  TraceEvent(Type type,
             Category category,
             const std::string& name,
             Arguments&& args);

  static void MarkDurationBegin(Category category, const std::string& name);
  static void MarkDurationEnd(Category category, const std::string& name);
  static void MarkInstant(Category category, const std::string& name);
  static void MarkAsyncStart(Category category, const std::string& name);
  static void MarkAsyncInstant(Category category, const std::string& name);
  static void MarkAsyncEnd(Category category, const std::string& name);
  static void MarkCounter(Category category,
                          const std::string& name,
                          int64_t count);

  void recordToStream(int pid, std::thread::id tid, std::stringstream& stream);

 private:
  Type _type;
  Category _category;
  std::string _name;
  TraceClockPoint _timestamp;
  Arguments _arguments;

  RL_DISALLOW_COPY_AND_ASSIGN(TraceEvent);
};

class AutoDurationEvent {
 public:
  AutoDurationEvent(TraceEvent::Category category, const std::string& name)
      : _name(name), _category(category) {
    TraceEvent::MarkDurationBegin(category, name);
  }

  ~AutoDurationEvent() { TraceEvent::MarkDurationEnd(_category, _name); }

 private:
  std::string _name;
  TraceEvent::Category _category;

  RL_DISALLOW_COPY_AND_ASSIGN(AutoDurationEvent);
};

}  // namespace instrumentation
}  // namespace rl

#endif  // RADARLOVE_INSTRUMENTATION_TRACEEVENT_H_
