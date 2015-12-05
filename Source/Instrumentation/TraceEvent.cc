// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Instrumentation/TraceEvent.h>
#include <Instrumentation/Trace.h>

#include <sstream>

namespace rl {
namespace instrumentation {

TraceEvent::TraceEvent(Type type, Category category, const std::string& name)
    : _type(type),
      _category(category),
      _name(name),
      _timestamp(std::chrono::time_point_cast<TraceClockDuration>(
          core::Clock::now())) {
}

void TraceEvent::Record(Type type, Category category, const std::string& name) {
  ThreadTrace::Current().recordEvent(type, category, name);
}

template <class T>
static void StreamAppend(std::stringstream& stream,
                         const std::string& key,
                         const T& value,
                         bool escapeValue,
                         bool comma) {
  stream << "\"" << key << "\":";

  if (escapeValue) {
    stream << "\"";
  }

  stream << value;

  if (escapeValue) {
    stream << "\"";
  }

  if (comma) {
    stream << ",";
  }
}

static std::string NameForCategory(TraceEvent::Category category) {
  switch (category) {
    case TraceEvent::Category::Default:
      return "Default";
  }
}

void TraceEvent::recordToStream(int pid,
                                std::thread::id tid,
                                std::stringstream& stream) {
  stream << "{";
  StreamAppend(stream, "name", _name, true, true);
  StreamAppend(stream, "cat", NameForCategory(_category), true, true);
  StreamAppend(stream, "ph", static_cast<char>(_type), true, true);
  StreamAppend(stream, "pid", std::to_string(pid), false, true);
  StreamAppend(stream, "tid", tid /* thread IDs are printable */, true, true);
  StreamAppend(stream, "ts",
               std::to_string(_timestamp.time_since_epoch().count()), false,
               false);
  stream << "}";
}

}  // namespace instrumentation
}  // namespace rl
