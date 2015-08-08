// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_INTERFACE_INTERFACETRANSACTION__
#define __RADARLOVE_INTERFACE_INTERFACETRANSACTION__

#include <Core/Core.h>
#include <Compositor/Patch.h>

namespace rl {
class InterfaceTransaction {
 public:
  InterfaceTransaction();

  Patch& patch();

  /**
   *  Commit the updates made within the current transaction to the specified
   *  channel
   */
  void commit(Channel& channel);

 private:
  Patch _patch;

  DISALLOW_COPY_AND_ASSIGN(InterfaceTransaction);
};
}

#endif /* defined(__RADARLOVE_INTERFACE_INTERFACETRANSACTION__) */
