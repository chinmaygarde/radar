// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_COMPOSITOR_ENTITY_LEASE_
#define RADARLOVE_COMPOSITOR_ENTITY_LEASE_

#include <Core/Core.h>
#include <Compositor/EntityArena.h>

namespace rl {
class EntityLease {
 public:
  explicit EntityLease(size_t entityCount);

  std::shared_ptr<core::EventLoopSource> writeNotificationSource() const;

  EntityArena& readArena() const;
  EntityArena& swapReadArena();

  EntityArena& writeArena() const;
  EntityArena& swapWriteArena();

 private:
  const size_t _entityCount;
  core::SharedMemory _sharedMemory;
  std::shared_ptr<core::EventLoopSource> _writeNotificationSource;
  std::unique_ptr<EntityArena> _readArena;
  std::unique_ptr<EntityArena> _writeArena;

  std::unique_ptr<EntityArena> swapRead();
  std::unique_ptr<EntityArena> swapWrite();

  RL_DISALLOW_COPY_AND_ASSIGN(EntityLease);
};
}  // namespace rl

#endif  // RADARLOVE_COMPOSITOR_ENTITY_LEASE_
