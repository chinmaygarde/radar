// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE__WAITSET__
#define __RADARLOVE__WAITSET__

#include <set>

#include "Base.h"

namespace rl {

class LooperSource;

class WaitSet {
 public:
  typedef int Handle;

  WaitSet();
  ~WaitSet();

  bool addSource(std::shared_ptr<LooperSource> source);
  bool removeSource(std::shared_ptr<LooperSource> source);

  LooperSource* wait();

 private:
  Handle _handle;

  static Handle platformHandleCreate();
  static LooperSource* platformHandleWait(Handle handle);
  static void platformHandleDestory(Handle handle);

  std::set<std::shared_ptr<LooperSource>> _sources;

  DISALLOW_COPY_AND_ASSIGN(WaitSet);
};
}

#endif /* defined(__RADARLOVE__WAITSET__) */
