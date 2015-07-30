// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE__LOOPERSOURCE__
#define __RADARLOVE__LOOPERSOURCE__

#include "Core/Base.h"

#include <functional>
#include <utility>
#include <chrono>

#include "WaitSet.h"

namespace rl {

class LooperSource {
 public:
  typedef int Handle;
  typedef std::pair<Handle, Handle> Handles;

  typedef std::function<void(Handle)> IOHandler;

  typedef std::function<void(void)> WakeFunction;

  typedef std::function<Handles(void)> IOHandlesAllocator;
  typedef std::function<void(Handles)> IOHandlesDeallocator;

  typedef std::function<void(LooperSource* source,
                             WaitSet::Handle waitsetHandle,
                             Handle readHandle,
                             bool adding)> WaitSetUpdateHandler;

  LooperSource(IOHandlesAllocator handleAllocator,
               IOHandlesDeallocator handleDeallocator,
               IOHandler readHandler,
               IOHandler writeHandler) {
    _handlesAllocator = handleAllocator;
    _handlesDeallocator = handleDeallocator;

    _customWaitSetUpdateHandler = nullptr;

    _readHandler = readHandler;
    _writeHandler = writeHandler;

    _wakeFunction = nullptr;
    _handles = Handles(-1, -1);
    _handlesAllocated = false;
  }

  ~LooperSource() {
    if (_handlesAllocated && _handlesDeallocator != nullptr) {
      _handlesDeallocator(_handles);
    }
  }

  void onAwoken() {
    if (_wakeFunction != nullptr) {
      _wakeFunction();
    }
  }

  /*
   *  Wait Function
   */

  void setWakeFunction(WakeFunction wakeFunction) {
    _wakeFunction = wakeFunction;
  }

  WakeFunction wakeFunction() const { return _wakeFunction; }

  /*
   *  Accessing the handles
   */

  Handles handles() {
    /*
     *  Handles are allocated lazily
     */
    if (!_handlesAllocated) {
      auto allocator = _handlesAllocator;

      if (allocator != nullptr) {
        _handles = _handlesAllocator();
      }

      _handlesAllocated = true;
    }

    return _handles;
  }

  /*
   *  Reading the Signalled Source
   */

  Handle readHandle() { return handles().first; }

  IOHandler reader() { return _readHandler; }

  /*
   *  Writing to the Source for Signalling
   */

  Handle writeHandle() { return handles().second; }

  IOHandler writer() { return _writeHandler; }

  /*
   *  Interacting with a WaitSet
   */
  void updateInWaitSetHandle(WaitSet::Handle handle, bool shouldAdd);

  void setCustomWaitSetUpdateHandler(WaitSetUpdateHandler handler) {
    _customWaitSetUpdateHandler = handler;
  }

  WaitSetUpdateHandler customWaitSetUpdateHandler() const {
    return _customWaitSetUpdateHandler;
  }

  /*
   *  Utility methods for creating commonly used sources
   */

  static std::shared_ptr<LooperSource> AsTimer(
      std::chrono::nanoseconds repeatInterval);

  static std::shared_ptr<LooperSource> AsTrivial();

 private:
  IOHandlesAllocator _handlesAllocator;
  IOHandlesDeallocator _handlesDeallocator;

  Handles _handles;

  IOHandler _readHandler;
  IOHandler _writeHandler;

  WaitSetUpdateHandler _customWaitSetUpdateHandler;

  WakeFunction _wakeFunction;

  bool _handlesAllocated;

  DISALLOW_COPY_AND_ASSIGN(LooperSource);
};
}

#endif /* defined(__RADARLOVE__LOOPERSOURCE__) */
