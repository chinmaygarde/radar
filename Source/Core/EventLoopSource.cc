// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Macros.h>
#include <Core/EventLoopSource.h>
#include <Core/Utilities.h>

#include <unistd.h>

namespace rl {

EventLoopSource::EventLoopSource(RWHandlesProvider handleProvider,
                                 RWHandlesCollector handleCollector,
                                 IOHandler readHandler,
                                 IOHandler writeHandler,
                                 WaitSetUpdateHandler waitsetUpdateHandler) {
  _handlesProvider = handleProvider;
  _handlesCollector = handleCollector;

  _customWaitSetUpdateHandler = nullptr;

  _readHandler = readHandler;
  _writeHandler = writeHandler;

  _wakeFunction = nullptr;
  _handles = Handles(-1, -1);
  _handlesAllocated = false;
  _customWaitSetUpdateHandler = waitsetUpdateHandler;
}

EventLoopSource::~EventLoopSource() {
  if (_handlesAllocated && _handlesCollector != nullptr) {
    _handlesCollector(_handles);
  }
}

void EventLoopSource::onAwoken() {
  if (_wakeFunction != nullptr) {
    _wakeFunction();
  }
}

void EventLoopSource::setWakeFunction(WakeFunction wakeFunction) {
  _wakeFunction = wakeFunction;
}

EventLoopSource::WakeFunction EventLoopSource::wakeFunction() const {
  return _wakeFunction;
}

EventLoopSource::Handles EventLoopSource::handles() {
  /*
   *  Handles are allocated lazily
   */
  if (!_handlesAllocated) {
    auto provider = _handlesProvider;

    if (provider != nullptr) {
      _handles = _handlesProvider();
    }

    _handlesAllocated = true;
  }

  return _handles;
}

EventLoopSource::Handle EventLoopSource::readHandle() {
  return handles().first;
}

EventLoopSource::Handle EventLoopSource::writeHandle() {
  return handles().second;
}

EventLoopSource::IOHandler EventLoopSource::reader() {
  return _readHandler;
}

EventLoopSource::IOHandler EventLoopSource::writer() {
  return _writeHandler;
}

std::shared_ptr<EventLoopSource> EventLoopSource::Trivial() {
  /*
   *  We are using a simple pipe but this should likely be something
   *  that coalesces multiple writes. Something like an event_fd on Linux
   */

  RWHandlesProvider provider = [] {
    int descriptors[2] = {0};

    RL_CHECK(::pipe(descriptors));

    return Handles(descriptors[0], descriptors[1]);
  };

  RWHandlesCollector collector = [](Handles h) {
    RL_CHECK(::close(h.first));
    RL_CHECK(::close(h.second));
  };

  static const char EventLoopWakeMessage[] = "w";

  IOHandler reader = [](Handle r) {
    char buffer[sizeof(EventLoopWakeMessage) / sizeof(char)];

    ssize_t size = RL_TEMP_FAILURE_RETRY(::read(r, &buffer, sizeof(buffer)));

    RL_ASSERT(size == sizeof(buffer));
  };

  IOHandler writer = [](Handle w) {

    ssize_t size = RL_TEMP_FAILURE_RETRY(
        ::write(w, EventLoopWakeMessage, sizeof(EventLoopWakeMessage)));

    RL_ASSERT(size == sizeof(EventLoopWakeMessage));
  };

  return std::make_shared<EventLoopSource>(provider, collector, reader, writer,
                                           nullptr);
}

}  // namespace rl
