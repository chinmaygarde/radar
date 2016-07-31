// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Config.h"

#if !RL_OS_WINDOWS

#include "PipeTrivialSource.h"

#include <fcntl.h>
#include <unistd.h>

namespace rl {
namespace core {

std::shared_ptr<EventLoopSource> MakePipeBasedTrivialSource() {
  /*
   *  Theory of Operation:
   *  A non blocking pipe is setup to service trivial sources. When the reader
   *  is signalled, it keeps attempting reads till an EAGAIN is encountered.
   */

  EventLoopSource::RWHandlesProvider provider = []() {
    int descriptors[2] = {0};
    RL_CHECK(::pipe(descriptors));

    /*
     *  EINTRs only occur on F_SETLKW cmds
     */
    RL_CHECK(::fcntl(descriptors[0], F_SETFL, O_NONBLOCK));
    RL_CHECK(::fcntl(descriptors[1], F_SETFL, O_NONBLOCK));

    return EventLoopSource::Handles(descriptors[0], descriptors[1]);
  };

  EventLoopSource::RWHandlesCollector collector =
      [](EventLoopSource::Handles handles) {
        RL_TEMP_FAILURE_RETRY_AND_CHECK(
            ::close(static_cast<int>(handles.readHandle)));
        RL_TEMP_FAILURE_RETRY_AND_CHECK(
            ::close(static_cast<int>(handles.writeHandle)));
      };

  static const char payload[] = {'W'};

  EventLoopSource::IOHandler reader = [](EventLoopSource::Handle readHandle) {
    char buffer[1] = {0};
    const auto expected = sizeof(buffer);
    static_assert(sizeof(payload) == sizeof(buffer), "");

    size_t readCount = 0;

    while (true) {
      auto read = RL_TEMP_FAILURE_RETRY(
          ::read(static_cast<int>(readHandle), buffer, expected));

      if (read == -1 && errno == EAGAIN) {
        break;
      }

      if (read != expected) {
        return IOResult::Failure;
      }

      readCount++;
    }

    return readCount == 0 ? IOResult::Timeout : IOResult::Success;
  };

  EventLoopSource::IOHandler writer = [](EventLoopSource::Handle writeHandle) {
    /*
     *  Unconditionally write the payload on the pipe
     */
    const auto expected = sizeof(payload);
    auto written = RL_TEMP_FAILURE_RETRY(
        ::write(static_cast<int>(writeHandle), payload, expected));

    return expected == written ? IOResult::Success : IOResult::Failure;
  };

  return std::make_shared<EventLoopSource>(provider, collector, reader, writer,
                                           nullptr);
}

}  // namespace core
}  // namespace rl

#endif  // !RL_OS_WINDOWS
