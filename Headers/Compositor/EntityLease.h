// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_COMPOSITOR_ENTITY_LEASE__
#define __RADARLOVE_COMPOSITOR_ENTITY_LEASE__

#include <Core/Core.h>
#include <Compositor/EntityArena.h>

namespace rl {
class EntityLease {
 public:
  EntityLease(size_t entityCount);

  std::shared_ptr<EventLoopSource> writeNotificationSource() const;

  EntityArena& accessReadArena(bool swap);
  const EntityArena& accessWriteArena(bool swap, bool notify);

 private:
  const size_t _entityCount;
  SharedMemory _sharedMemory;
  std::shared_ptr<EventLoopSource> _writeNotificationSource;
  EntityArena _readArena;
  EntityArena _writeArena;

  EntityArena swapRead();
  EntityArena swapWrite(bool notify);
  uint8_t* swapWrite();

  DISALLOW_COPY_AND_ASSIGN(EntityLease);
};
}  // namespace rl

#endif /* defined(__RADARLOVE_COMPOSITOR_ENTITY_LEASE__) */
