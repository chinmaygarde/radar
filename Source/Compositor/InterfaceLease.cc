// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/InterfaceLease.h>
#include <Compositor/PresentationEntity.h>
#include <atomic>

namespace rl {
struct LeaseHeader {
  std::atomic<uint8_t*> readBuffer;
  std::atomic<uint8_t*> writeBuffer;
  std::atomic<uint8_t*> backBuffer;
};

static_assert(sizeof(std::atomic<uint8_t*>) == sizeof(uint8_t*),
              "Sizes must be consistent");

InterfaceLease::InterfaceLease(size_t requested_count)
    : _layerCount(requested_count + 1),
      _sharedMemory(sizeof(LeaseHeader) +
                    (3 * _layerCount * sizeof(PresentationEntity))),
      _writeNotificationSource(LooperSource::AsTrivial()) {
  assert(_sharedMemory.isReady());

  auto header = reinterpret_cast<LeaseHeader*>(_sharedMemory.address());
  header->readBuffer = _sharedMemory.address() + sizeof(LeaseHeader);
  header->writeBuffer =
      header->readBuffer + _layerCount * sizeof(PresentationEntity);
  header->backBuffer =
      header->writeBuffer + _layerCount * sizeof(PresentationEntity);
}

EntityArena InterfaceLease::swapRead() {
  auto header = reinterpret_cast<LeaseHeader*>(_sharedMemory.address());
  header->backBuffer = header->readBuffer.exchange(header->backBuffer);
  return EntityArena(header->readBuffer,
                     _layerCount * sizeof(PresentationEntity), true);
}

EntityArena InterfaceLease::swapWriteAndNotify(bool notify) {
  auto ret = swapWrite();
  if (notify) {
    _writeNotificationSource->writer()(_writeNotificationSource->writeHandle());
  }
  return EntityArena(ret, _layerCount * sizeof(PresentationEntity), false);
}

uint8_t* InterfaceLease::swapWrite() {
  auto header = reinterpret_cast<LeaseHeader*>(_sharedMemory.address());
  header->backBuffer = header->writeBuffer.exchange(header->backBuffer);
  return header->writeBuffer;
}

std::shared_ptr<LooperSource> InterfaceLease::writeNotificationSource() const {
  return _writeNotificationSource;
}

}  // namespace rl
