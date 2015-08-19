// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/EntityLease.h>
#include <Compositor/PresentationEntity.h>
#include <atomic>

namespace rl {
struct LeaseHeader {
  std::atomic<uint8_t*> read;
  std::atomic<uint8_t*> write;
  std::atomic<uint8_t*> back;
};

static_assert(sizeof(std::atomic<uint8_t*>) == sizeof(uint8_t*),
              "Sizes must be consistent");

/**
 *  Returns the size of the shared memory allocation required to service a given
 *  number of presentation entities
 *
 *  @param entityCount the number of entities to service
 *
 *  @return the size of the allocation
 */
static inline size_t EntityLease_SharedMemorySize(size_t entityCount) {
  /*
   *  The shared memory allocation is triple buffered. So we need three times
   *  the size of the expected entity arenas. Also, the pointers to the buffers
   *  are stored in the lease headers. This header needs space in the
   *  allocation as well.
   */
  return sizeof(LeaseHeader) + (3 * EntityArena::Size(entityCount));
}

EntityLease::EntityLease(size_t requestedCount)
    : _entityCount(requestedCount),
      _sharedMemory(EntityLease_SharedMemorySize(requestedCount)),
      _writeNotificationSource(EventLoopSource::Trivial()),
      _readArena(nullptr, 0, true),
      _writeArena(nullptr, 0, false) {
  RL_ASSERT(_sharedMemory.isReady());

  auto header = reinterpret_cast<LeaseHeader*>(_sharedMemory.address());
  header->read = _sharedMemory.address() + sizeof(LeaseHeader);
  header->write = header->read + EntityArena::Size(_entityCount);
  header->back = header->write + EntityArena::Size(_entityCount);

  accessReadArena(true);
  accessWriteArena(true, false);
}

EntityArena& EntityLease::accessReadArena(bool swap) {
  if (!swap) {
    return _readArena;
  }

  _readArena = swapRead();
  return _readArena;
}

const EntityArena& EntityLease::accessWriteArena(bool swap, bool notify) {
  if (!swap) {
    return _writeArena;
  }

  _writeArena = swapWrite(notify);
  return _writeArena;
}

EntityArena EntityLease::swapRead() {
  auto header = reinterpret_cast<LeaseHeader*>(_sharedMemory.address());
  header->back = header->read.exchange(header->back);
  return EntityArena(header->read, EntityArena::Size(_entityCount), true);
}

EntityArena EntityLease::swapWrite(bool notify) {
  auto ret = swapWrite();
  if (notify) {
    _writeNotificationSource->writer()(_writeNotificationSource->writeHandle());
  }
  return EntityArena(ret, EntityArena::Size(_entityCount), false);
}

uint8_t* EntityLease::swapWrite() {
  auto header = reinterpret_cast<LeaseHeader*>(_sharedMemory.address());
  header->back = header->write.exchange(header->back);
  return header->write;
}

std::shared_ptr<EventLoopSource> EntityLease::writeNotificationSource() const {
  return _writeNotificationSource;
}

}  // namespace rl
