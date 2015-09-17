// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Macros.h>
#include <Core/EventLoopSource.h>
#include <Core/Utilities.h>

#include <unistd.h>

#define HANDLE_CAST(x) static_cast<int>((x))

namespace rl {

EventLoopSource::EventLoopSource(RWHandlesProvider handleProvider,
                                 RWHandlesCollector handleCollector,
                                 IOHandler readHandler,
                                 IOHandler writeHandler,
                                 WaitSetUpdateHandler waitsetUpdateHandler)
    : _handlesProvider(handleProvider),
      _handlesCollector(handleCollector),
      _readHandler(readHandler),
      _writeHandler(writeHandler),
      _customWaitSetUpdateHandler(waitsetUpdateHandler),
      _wakeFunction(nullptr),
      _handles(Handles(-1, -1)),
      _handlesAllocated(false) {
}

EventLoopSource::~EventLoopSource() {
  if (_handlesAllocated && _handlesCollector) {
    _handlesCollector(_handles);
  }
}

void EventLoopSource::onAwoken() {
  if (_wakeFunction) {
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
    if (_handlesProvider) {
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

EventLoopSource::IOHandler EventLoopSource::reader() const {
  return _readHandler;
}

void EventLoopSource::setReader(const IOHandler& reader) {
  _readHandler = reader;
}

EventLoopSource::IOHandler EventLoopSource::writer() const {
  return _writeHandler;
}

void EventLoopSource::setWriter(const IOHandler& writer) {
  _writeHandler = writer;
}

void EventLoopSource::updateInWaitSetHandle(WaitSet::Handle waitsetHandle,
                                            bool shouldAdd) {
  if (_customWaitSetUpdateHandler) {
    _customWaitSetUpdateHandler(*this, waitsetHandle, readHandle(), shouldAdd);
  } else {
    updateInWaitSetHandleForSimpleRead(waitsetHandle, shouldAdd);
  }
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
    RL_CHECK(::close(HANDLE_CAST(h.first)));
    RL_CHECK(::close(HANDLE_CAST(h.second)));
  };

  static const char EventLoopWakeMessage[] = "w";

  IOHandler reader = [](Handle r) {
    char buffer[sizeof(EventLoopWakeMessage) / sizeof(char)];

    ssize_t size =
        RL_TEMP_FAILURE_RETRY(::read(HANDLE_CAST(r), &buffer, sizeof(buffer)));

    RL_ASSERT(size == sizeof(buffer));
  };

  IOHandler writer = [](Handle w) {

    ssize_t size = RL_TEMP_FAILURE_RETRY(::write(
        HANDLE_CAST(w), EventLoopWakeMessage, sizeof(EventLoopWakeMessage)));

    RL_ASSERT(size == sizeof(EventLoopWakeMessage));
  };

  return std::make_shared<EventLoopSource>(provider, collector, reader, writer,
                                           nullptr);
}

}  // namespace rl
