// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/ThreadLocal.h>

namespace rl {
namespace core {

#if RL_THREAD_LOCAL_PTHREADS

void ThreadLocal::ThreadLocalDestroy(Box* value) {
  RL_ASSERT(value != nullptr);
  value->destroyValue();
  delete value;
}

#endif

}  // namespace core
}  // namespace rl
