// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_INSTRUMENTATION_TRACEEVENT_H_
#define RADARLOVE_INSTRUMENTATION_TRACEEVENT_H_

#include <Core/Core.h>

#include <map>
#include <thread>

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

}  // namespace instrumentation
}  // namespace rl

#endif  // RADARLOVE_INSTRUMENTATION_TRACEEVENT_H_
