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

  TraceEvent(Type type, Category category, const std::string& name);

  static void Record(Type type, Category category, const std::string& name);

  void recordToStream(int pid, std::thread::id tid, std::stringstream& stream);

 private:
  Type _type;
  Category _category;
  std::string _name;
  TraceClockPoint _timestamp;
  std::map<std::string, std::string> _arguments;

  RL_DISALLOW_COPY_AND_ASSIGN(TraceEvent);
};

}  // namespace instrumentation
}  // namespace rl

#endif  // RADARLOVE_INSTRUMENTATION_TRACEEVENT_H_
