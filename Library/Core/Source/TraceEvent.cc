/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/Trace.h>
#include <Core/TraceEvent.h>
#include <sstream>

namespace rl {
namespace instrumentation {

static inline TraceEvent::TraceClockPoint CurrentTraceTimestamp() {
  return std::chrono::time_point_cast<TraceEvent::TraceClockDuration>(
      core::Clock::now());
}

TraceEvent::TraceEvent(Type type,
                       Category category,
                       const std::string& name,
                       Arguments&& args)
    : _type(type),
      _category(category),
      _name(name),
      _timestamp(CurrentTraceTimestamp()),
      _arguments(std::move(args)) {}

void TraceEvent::MarkDurationBegin(Category category, const std::string& name) {
  ThreadTrace::Current().recordEvent(Type::DurationBegin, category, name,
                                     Arguments());
}

void TraceEvent::MarkDurationEnd(Category category, const std::string& name) {
  ThreadTrace::Current().recordEvent(Type::DurationEnd, category, name,
                                     Arguments());
}

void TraceEvent::MarkInstant(Category category, const std::string& name) {
  ThreadTrace::Current().recordEvent(Type::Instant, category, name,
                                     Arguments());
}

void TraceEvent::MarkAsyncStart(Category category, const std::string& name) {
  ThreadTrace::Current().recordEvent(Type::AsyncStart, category, name,
                                     Arguments());
}

void TraceEvent::MarkAsyncInstant(Category category, const std::string& name) {
  ThreadTrace::Current().recordEvent(Type::AsyncInstant, category, name,
                                     Arguments());
}

void TraceEvent::MarkAsyncEnd(Category category, const std::string& name) {
  ThreadTrace::Current().recordEvent(Type::AsyncEnd, category, name,
                                     Arguments());
}

void TraceEvent::MarkMetadata(const std::string& name, Arguments&& args) {
  ThreadTrace::Current().recordEvent(Type::Metadata, Category::Default, name,
                                     std::move(args));
}

void TraceEvent::MarkCounter(Category category,
                             const std::string& name,
                             int64_t count) {
  ThreadTrace::Current().recordEvent(
      Type::Counter, category, name,
      Arguments({{name, std::to_string(count)}}));
}

template <class T>
static void StreamAppend(std::stringstream& stream,
                         const std::string& key,
                         const T& value,
                         bool comma) {
  stream << "\"" << key << "\":\"" << value << "\"";

  if (comma) {
    stream << ",";
  }
}

static std::string NameForCategory(TraceEvent::Category category) {
  switch (category) {
    case TraceEvent::Category::Default:
      return "Default";
  }
  RL_UNREACHABLE;
  return "";
}

void TraceEvent::recordToStream(int pid,
                                std::thread::id tid,
                                std::stringstream& stream) {
  auto argsSize = _arguments.size();
  auto hasExtraArgs = argsSize > 0;
  auto timestamp = _timestamp.time_since_epoch().count();

  stream << "{";

  /*
   *  All instant events are treated as process scoped
   */
  if (_type == Type::Instant) {
    StreamAppend(stream, "s", "p", true);
  }

  StreamAppend(stream, "name", _name, true);
  StreamAppend(stream, "cat", NameForCategory(_category), true);
  StreamAppend(stream, "ph", static_cast<char>(_type), true);
  StreamAppend(stream, "pid", std::to_string(pid), true);
  StreamAppend(stream, "tid", tid /* thread IDs are printable */, true);
  StreamAppend(stream, "ts", std::to_string(timestamp), hasExtraArgs);

  if (hasExtraArgs) {
    stream << "\"args\":{";
    size_t current = 0;

    for (auto& item : _arguments) {
      StreamAppend(stream, item.first, item.second, ++current != argsSize);
    }

    stream << "}";
  }

  stream << "}";
}

const char* TraceEventMetadataNameKey = "name";
const char* TraceEventMetadataThreadName = "thread_name";
const char* TraceEventMetadataProcessName = "process_name";

}  // namespace instrumentation
}  // namespace rl
