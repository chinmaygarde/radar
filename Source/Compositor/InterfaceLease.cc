// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/InterfaceLease.h>
#include <Compositor/PresentationLayer.h>
#include <atomic>

namespace rl {
struct LeaseHeader {
  std::atomic<uint8_t*> readBuffer;
  std::atomic<uint8_t*> writeBuffer;
  std::atomic<uint8_t*> backBuffer;
};

static_assert(sizeof(std::atomic<uint8_t*>) == sizeof(uint8_t*),
              "Sizes must be consistent");

InterfaceLease::InterfaceLease(size_t layerCount)
    : _sharedMemory(sizeof(LeaseHeader) +
                    (3 * layerCount * sizeof(PresentationLayer))),
      _writeNotificationSource(LooperSource::AsTrivial()) {
  assert(_sharedMemory.isReady());

  auto header = reinterpret_cast<LeaseHeader*>(_sharedMemory.address());
  header->readBuffer = _sharedMemory.address() + sizeof(LeaseHeader);
  header->writeBuffer =
      header->readBuffer + layerCount * sizeof(PresentationLayer);
  header->backBuffer =
      header->writeBuffer + layerCount * sizeof(PresentationLayer);
}

uint8_t* InterfaceLease::swapRead() {
  auto header = reinterpret_cast<LeaseHeader*>(_sharedMemory.address());
  header->backBuffer = header->readBuffer.exchange(header->backBuffer);
  return header->readBuffer;
}

uint8_t* InterfaceLease::swapWriteAndNotify() {
  auto ret = swapWrite();
  _writeNotificationSource->writer()(_writeNotificationSource->writeHandle());
  return ret;
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
