// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Core/WaitSet.h"
#include "Core/Utilities.h"

#include <sys/event.h>
#include <unistd.h>

using namespace rl;

WaitSet::Handle WaitSet::platformHandleCreate() {
  WaitSet::Handle handle = RL_TEMP_FAILURE_RETRY(::kqueue());

  RL_ASSERT(handle != -1);

  return handle;
}

LooperSource* WaitSet::platformHandleWait(WaitSet::Handle handle) {
  struct kevent event = {0};

  int val =
      RL_TEMP_FAILURE_RETRY(::kevent(handle, nullptr, 0, &event, 1, nullptr));

  RL_ASSERT(val == 1);

  return static_cast<LooperSource*>(event.udata);
}

void WaitSet::platformHandleDestory(WaitSet::Handle handle) {
  RL_CHECK(::close(handle));
}
