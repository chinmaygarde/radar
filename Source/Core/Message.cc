// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Message.h>
#include <Core/Utilities.h>

#include <stdlib.h>

#if RL_OS_MAC
#include <mach/mach.h>
#endif

#include <sys/mman.h>

namespace rl {

Message::Message(size_t length)
    : _buffer(nullptr), _bufferLength(0), _dataLength(0), _sizeRead(0) {
  reserve(length);
}

Message::Message(uint8_t* buffer, size_t bufferSize)
    : _bufferLength(bufferSize), _dataLength(bufferSize), _sizeRead(0) {
  void* allocation = malloc(bufferSize);
  memcpy(allocation, buffer, bufferSize);
  _buffer = static_cast<uint8_t*>(allocation);
}

Message::Message(uint8_t* buffer, size_t bufferLength, bool vmDeallocate)
    : _buffer(buffer),
      _bufferLength(bufferLength),
      _dataLength(bufferLength),
      _sizeRead(0),
      _vmDeallocate(vmDeallocate) {
  RL_ASSERT(vmDeallocate);
}

Message::Message(Message&& message) noexcept
    : _buffer(message._buffer),
      _bufferLength(message._bufferLength),
      _dataLength(message._dataLength),
      _sizeRead(message._sizeRead),
      _vmDeallocate(message._vmDeallocate) {
  message._buffer = nullptr;
}

Message::~Message() {
  if (_vmDeallocate) {
    if (_buffer) {
#if RL_OS_MAC
      kern_return_t res =
          vm_deallocate(mach_task_self(),
                        reinterpret_cast<vm_address_t>(_buffer), _bufferLength);
      RL_ASSERT(res == KERN_SUCCESS);
#elif RL_OS_LINUX || RL_OS_NACL
      RL_CHECK(::munmap(_buffer, _bufferLength));
#else
#error Unknown Platform
#endif
    }
  } else {
    free(_buffer);
  }
}

static inline size_t Message_NextPOTSize(size_t x) {
  --x;

  x |= x >> 1;
  x |= x >> 2;
  x |= x >> 4;
  x |= x >> 8;
  x |= x >> 16;

  return x + 1;
}

bool Message::reserve(size_t length) {
  if (length == 0 || _bufferLength >= length) {
    return true;
  }

  return resizeBuffer(Message_NextPOTSize(length));
}

bool Message::resizeBuffer(size_t size) {
  if (_buffer == nullptr) {
    RL_ASSERT(_bufferLength == 0);

    void* buffer = malloc(size);

    bool success = buffer != nullptr;

    if (success) {
      _buffer = static_cast<uint8_t*>(buffer);
      _bufferLength = size;
    }

    return success;
  }

  RL_ASSERT(size > _bufferLength);

  void* resized = realloc(_buffer, size);

  bool success = resized != nullptr;

  if (success) {
    _buffer = static_cast<uint8_t*>(resized);
    _bufferLength = size;
  }

  return success;
}

uint8_t* Message::encodeRawUnsafe(size_t size) {
  bool success = reserve(_dataLength + size);

  if (!success) {
    return nullptr;
  }

  auto oldLength = _dataLength;
  _dataLength += size;
  return _buffer + oldLength;
}

size_t Message::encodeRawOffsetUnsafe(size_t size) {
  if (encodeRawUnsafe(size) == nullptr) {
    return std::numeric_limits<size_t>::max();
  }

  return _dataLength - size;
}

uint8_t* Message::decodeRawUnsafe(size_t size) {
  if ((size + _sizeRead) > _bufferLength) {
    return nullptr;
  }

  auto buffer = _buffer + _sizeRead;
  _sizeRead += size;
  return buffer;
}

uint8_t* Message::operator[](size_t index) {
  if (index >= _dataLength) {
    return nullptr;
  }

  return _buffer + index;
}

uint8_t* Message::data() const {
  return _buffer;
}

size_t Message::size() const {
  return _dataLength;
}

size_t Message::sizeRead() const {
  return _sizeRead;
}

bool Message::readCompleted() const {
  return _sizeRead == _dataLength;
}

}  // namespace rl
