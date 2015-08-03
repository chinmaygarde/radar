// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Core/Message.h"
#include "Core/Utilities.h"

#include <stdlib.h>

namespace rl {

Message::Message(size_t length)
    : _buffer(nullptr), _bufferLength(0), _dataLength(0), _sizeRead(0) {
  reserve(length);
}

Message::Message(const uint8_t* buffer, size_t bufferSize)
    : _bufferLength(bufferSize), _dataLength(bufferSize), _sizeRead(0) {
  void* allocation = malloc(bufferSize);

  memcpy(allocation, buffer, bufferSize);

  _buffer = static_cast<uint8_t*>(allocation);
}

Message::~Message() {
  free(_buffer);
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

template <typename T>
bool Message::encode(T value) {
  bool success = reserve(_dataLength + sizeof(T));

  if (success) {
    memcpy(_buffer + _dataLength, &value, sizeof(T));
    _dataLength += sizeof(T);
  }

  return success;
}

template <typename T>
bool Message::decode(T& t) {
  if ((sizeof(T) + _sizeRead) > _bufferLength) {
    return false;
  }

  memcpy(&t, _buffer + _sizeRead, sizeof(T));

  _sizeRead += sizeof(T);

  return true;
}

const uint8_t* Message::data() const {
  return _buffer;
}

size_t Message::size() const {
  return _dataLength;
}

size_t Message::sizeRead() const {
  return _sizeRead;
}

}  // namespace rl
