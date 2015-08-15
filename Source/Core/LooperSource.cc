// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Macros.h>
#include <Core/LooperSource.h>
#include <Core/Utilities.h>

#include <unistd.h>

namespace rl {

LooperSource::LooperSource(RWHandlesProvider handleProvider,
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

LooperSource::~LooperSource() {
  if (_handlesAllocated && _handlesCollector != nullptr) {
    _handlesCollector(_handles);
  }
}

void LooperSource::onAwoken() {
  if (_wakeFunction != nullptr) {
    _wakeFunction();
  }
}

void LooperSource::setWakeFunction(WakeFunction wakeFunction) {
  _wakeFunction = wakeFunction;
}

LooperSource::WakeFunction LooperSource::wakeFunction() const {
  return _wakeFunction;
}

LooperSource::Handles LooperSource::handles() {
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

LooperSource::Handle LooperSource::readHandle() {
  return handles().first;
}

LooperSource::Handle LooperSource::writeHandle() {
  return handles().second;
}

LooperSource::IOHandler LooperSource::reader() {
  return _readHandler;
}

LooperSource::IOHandler LooperSource::writer() {
  return _writeHandler;
}

std::shared_ptr<LooperSource> LooperSource::AsTrivial() {
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

  static const char LooperWakeMessage[] = "w";

  IOHandler reader = [](Handle r) {
    char buffer[sizeof(LooperWakeMessage) / sizeof(char)];

    ssize_t size = RL_TEMP_FAILURE_RETRY(::read(r, &buffer, sizeof(buffer)));

    RL_ASSERT(size == sizeof(buffer));
  };

  IOHandler writer = [](Handle w) {

    ssize_t size = RL_TEMP_FAILURE_RETRY(
        ::write(w, LooperWakeMessage, sizeof(LooperWakeMessage)));

    RL_ASSERT(size == sizeof(LooperWakeMessage));
  };

  return std::make_shared<LooperSource>(provider, collector, reader, writer,
                                        nullptr);
}

}  // namespace rl
