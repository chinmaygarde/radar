// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_CORE_WAITSETPROVIDER_H__
#define __RADARLOVE_CORE_WAITSETPROVIDER_H__

#include <Core/Macros.h>
#include <Core/WaitSet.h>
#include <Core/EventLoopSource.h>

namespace rl {

class WaitSetProvider {
 public:
  virtual EventLoopSource& wait() = 0;

  virtual WaitSet::Handle handle() const = 0;

  virtual void signalReadReadinessFromUserspace(
      EventLoopSource::Handle writeHandle);

  virtual void updateSource(WaitSet& waitset,
                            EventLoopSource& source,
                            bool addedOrRemoved);

  virtual ~WaitSetProvider();
};

}  // namespace rl

#endif  // __RADARLOVE_CORE_WAITSETPROVIDER_H__