// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Message.h>
#include <Core/Utilities.h>

#include <stdlib.h>

#if RL_OS_MAC
#include <mach/mach.h>
#elif RL_OS_LINUX || RL_OS_BSD
#include <sys/mman.h>
#elif RL_OS_WINDOWS || RL_OS_NACL
// No platform owned buffer implementation
#else
#error Unknown Platform
#endif

namespace rl {
namespace core {

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

Message::Message(Message&& message)
    : _buffer(message._buffer),
      _bufferLength(message._bufferLength),
      _dataLength(message._dataLength),
      _sizeRead(message._sizeRead),
      _vmDeallocate(message._vmDeallocate),
      _attachments(std::move(message._attachments)) {
  message._buffer = nullptr;
}

Message::Message(std::vector<Attachment>&& attachments)
    : _buffer(nullptr),
      _bufferLength(0),
      _dataLength(0),
      _sizeRead(0),
      _vmDeallocate(false),
      _attachments(std::move(attachments)) {}

Message::~Message() {
  if (_vmDeallocate) {
    if (_buffer) {
#if RL_OS_MAC
      kern_return_t res =
          vm_deallocate(mach_task_self(),
                        reinterpret_cast<vm_address_t>(_buffer), _bufferLength);
      RL_ASSERT(res == KERN_SUCCESS);
#elif RL_OS_LINUX || RL_OS_BSD
      RL_CHECK(::munmap(_buffer, _bufferLength));
#elif RL_OS_WINDOWS || RL_OS_NACL
      RL_ASSERT_MSG(false, "No platform owned buffer implementation");
#else
#error Unknown Platform
#endif
    }
  } else {
    free(_buffer);
  }
}

Message::Attachment::Attachment() : _handle(MessageAttachmentHandleNull) {}

Message::Attachment::Attachment(Message::Attachment::Handle handle)
    : _handle(handle) {}

bool Message::Attachment::isValid() const {
  return _handle != MessageAttachmentHandleNull;
}

Message::Attachment::Handle Message::Attachment::handle() const {
  return _handle;
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

bool Message::encode(const Serializable& value) {
  return value.serialize(*this);
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

size_t Message::encodeOffsetRawUnsafe(size_t size) {
  if (encodeRawUnsafe(size) == nullptr) {
    return std::numeric_limits<size_t>::max();
  }

  return _dataLength - size;
}

bool Message::decode(Serializable& value) {
  return value.deserialize(*this);
}

uint8_t* Message::decodeRawUnsafe(size_t size) {
  if ((size + _sizeRead) > _bufferLength) {
    return nullptr;
  }

  auto buffer = _buffer + _sizeRead;
  _sizeRead += size;
  return buffer;
}

uint8_t* Message::alignAllocation(uint8_t* allocation,
                                  size_t alignment,
                                  bool encoding) {
  if (allocation == nullptr) {
    return nullptr;
  }

  auto allocationPtr = reinterpret_cast<uintptr_t>(allocation);

  const auto extra = allocationPtr % alignment;

  if (extra != 0) {
    const auto snap = alignment - extra;
    uint8_t* snapAllocation = nullptr;

    if (encoding) {
      snapAllocation = encodeRawUnsafe(snap);
    } else {
      snapAllocation = decodeRawUnsafe(snap);
    }

    if (snapAllocation == nullptr) {
      return nullptr;
    }

    allocationPtr += snap;
  }

  return reinterpret_cast<uint8_t*>(allocationPtr);
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

void Message::rewindRead() {
  _sizeRead = 0;
}

const std::vector<Message::Attachment>& Message::attachments() const {
  return _attachments;
}

void Message::addAttachment(const Attachment& attachment) {
  if (attachment.isValid()) {
    _attachments.emplace_back(attachment);
  }
}

void Message::setAttachments(std::vector<Attachment>&& attachments) {
  _attachments = std::move(attachments);
}

const Message::Attachment::Handle MessageAttachmentHandleNull = 0;

}  // namespace core
}  // namespace rl
