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
      _writeNotificationSource(core::EventLoopSource::Trivial()),
      _readArena(nullptr),
      _writeArena(nullptr) {
  RL_ASSERT(_sharedMemory.isReady());

  /*
   *  Setup initial pointer offsets in shared memory
   */
  auto header = reinterpret_cast<LeaseHeader*>(_sharedMemory.address());
  header->read = _sharedMemory.address() + sizeof(LeaseHeader);
  header->write = header->read + EntityArena::Size(_entityCount);
  header->back = header->write + EntityArena::Size(_entityCount);

  /*
   *  Prepare initial arenas
   */
  _readArena = core::make_unique<EntityArena>(
      header->read, EntityArena::Size(_entityCount), true);
  _writeArena = core::make_unique<EntityArena>(
      header->write, EntityArena::Size(_entityCount), false);
}

EntityArena& EntityLease::readArena() const {
  return *_readArena;
}

EntityArena& EntityLease::swapReadArena() {
  _readArena = swapRead();
  return *_readArena;
}

EntityArena& EntityLease::writeArena() const {
  return *_writeArena;
}

EntityArena& EntityLease::swapWriteArena() {
  _writeArena = swapWrite();
  _writeNotificationSource->writer()(_writeNotificationSource->writeHandle());
  return *_writeArena;
}

std::unique_ptr<EntityArena> EntityLease::swapRead() {
  auto header = reinterpret_cast<LeaseHeader*>(_sharedMemory.address());
  header->back = header->read.exchange(header->back);
  return core::make_unique<EntityArena>(header->read,
                                        EntityArena::Size(_entityCount), true);
}

std::unique_ptr<EntityArena> EntityLease::swapWrite() {
  auto header = reinterpret_cast<LeaseHeader*>(_sharedMemory.address());
  header->back = header->write.exchange(header->back);
  return core::make_unique<EntityArena>(header->write,
                                        EntityArena::Size(_entityCount), false);
}

std::shared_ptr<core::EventLoopSource> EntityLease::writeNotificationSource()
    const {
  return _writeNotificationSource;
}

}  // namespace rl
