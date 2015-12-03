// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Macros.h>
#include <Core/EventLoopSource.h>
#include <Core/Utilities.h>

#define HANDLE_CAST(x) static_cast<int>((x))

namespace rl {
namespace core {

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
      _handles(Handles(-1, -1)),
      _handlesAllocated(false) {
}

EventLoopSource::EventLoopSource()
    : _handles(Handles(-1, -1)), _handlesAllocated(false) {
}

EventLoopSource::~EventLoopSource() {
  if (_handlesAllocated && _handlesCollector) {
    _handlesCollector(_handles);
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

void EventLoopSource::setHandlesProvider(RWHandlesProvider provider) {
  _handlesProvider = provider;
}

void EventLoopSource::setHandlesCollector(RWHandlesCollector collector) {
  _handlesCollector = collector;
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

void EventLoopSource::updateInWaitSet(WaitSet& waitset, bool shouldAdd) {
  if (_customWaitSetUpdateHandler) {
    _customWaitSetUpdateHandler(*this, waitset, readHandle(), shouldAdd);
  } else {
    updateInWaitSetForSimpleRead(waitset, shouldAdd);
  }
}

void EventLoopSource::setCustomWaitSetUpdateHandler(
    WaitSetUpdateHandler updateHandler) {
  _customWaitSetUpdateHandler = updateHandler;
}

void EventLoopSource::setReadAttemptCallback(ReadAttemptCallback callback) {
  _readAttemptCallback = callback;
}

EventLoopSource::ReadAttemptCallback EventLoopSource::readAttemptCallback()
    const {
  return _readAttemptCallback;
}

void EventLoopSource::attemptRead() {
  /*
   *  Acquire the read. Make sure to go through the accessor as it lazily
   *  initializes the handle.
   */
  auto readHandle = this->readHandle();

  /*
   *  First, ask the instance if it wants to read on the handle at this time
   */
  bool shouldAttemptRead =
      _readAttemptCallback ? _readAttemptCallback(readHandle) : true;

  auto result = IOHandlerResult::Timeout;

  if (shouldAttemptRead && _readHandler) {
    result = _readHandler(readHandle);
  }

  /*
   *  Perform wake callbacks for the source
   */
  if (_wakeFunction) {
    _wakeFunction(result);
  }
}

}  // namespace core
}  // namespace rl
