// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_COMPOSITOR_INTERFACELEASE__
#define __RADARLOVE_COMPOSITOR_INTERFACELEASE__

#include <Core/Core.h>

namespace rl {
class InterfaceLease {
 public:
  InterfaceLease(size_t layerCount);

  std::shared_ptr<LooperSource> writeNotificationSource() const;

  uint8_t* swapRead();
  uint8_t* swapWriteAndNotify();

 private:
  SharedMemory _sharedMemory;
  std::shared_ptr<LooperSource> _writeNotificationSource;

  uint8_t* swapWrite();

  DISALLOW_COPY_AND_ASSIGN(InterfaceLease);
};
}  // namespace rl

#endif /* defined(__RADARLOVE_COMPOSITOR_INTERFACELEASE__) */
