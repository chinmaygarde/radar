// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Core/LooperSource.h"
#include "Core/Utilities.h"
#include "Core/Base.h"

#include <unistd.h>

using namespace rl;

std::shared_ptr<LooperSource> LooperSource::AsTrivial() {
  /*
   *  We are using a simple pipe but this should likely be something
   *  that coalesces multiple writes. Something like an event_fd on Linux
   */

  IOHandlesAllocator allocator = [] {
    int descriptors[2] = {0};

    RL_CHECK(::pipe(descriptors));

    return Handles(descriptors[0], descriptors[1]);
  };

  IOHandlesDeallocator deallocator = [](Handles h) {
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

  return std::make_shared<LooperSource>(allocator, deallocator, reader, writer);
}
