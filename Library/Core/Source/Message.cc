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

#include <string>

namespace rl {
namespace core {

static inline uint8_t* MessageCreateBufferCopy(const uint8_t* src, size_t len) {
  if (src == nullptr || len == 0) {
    return nullptr;
  }

  void* allocation = malloc(len);

  if (allocation == nullptr) {
    return nullptr;
  }

  return reinterpret_cast<uint8_t*>(memmove(allocation, src, len));
}

Message::Message(size_t length) : Message(nullptr, 0, false) {
  reserve(length);
}

Message::Message(uint8_t* buffer, size_t bufferSize)
    : Message(buffer, bufferSize, false) {}

Message::Message(std::vector<Attachment>&& attachments)
    : Message(nullptr, 0, false) {
  _attachments = std::move(attachments);
}

Message::Message(uint8_t* buffer, size_t bufferLength, bool vmAllocated)
    : _buffer(vmAllocated ? buffer
                          : MessageCreateBufferCopy(buffer, bufferLength)),
      _bufferLength(_buffer == nullptr ? 0 : bufferLength),
      _dataLength(_bufferLength),
      _sizeRead(0),
      _vmAllocated(vmAllocated) {}

Message::Message(Message&& message)
    : _buffer(message._buffer),
      _bufferLength(message._bufferLength),
      _dataLength(message._dataLength),
      _sizeRead(message._sizeRead),
      _vmAllocated(message._vmAllocated),
      _attachments(std::move(message._attachments)) {
  message._buffer = nullptr;
}

Message::~Message() {
  if (_vmAllocated) {
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

bool Message::encode(const MessageSerializable& value) {
  return value.serialize(*this);
}

bool Message::encode(const std::string& value) {
  auto stringLength = value.size();
  /*
   *  Encode the string length
   */
  if (encode(stringLength)) {
    /*
     *  Reserve an adquate allocation
     */
    if (auto allocation = encodeRawUnsafe(stringLength)) {
      memmove(allocation, value.data(), stringLength);
      return true;
    }
  }
  return false;
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

bool Message::decode(MessageSerializable& value, Namespace* ns) {
  return value.deserialize(*this, ns);
}

bool Message::decode(std::string& string) {
  auto length = string.length();
  bool success = decode(length, nullptr);

  if (!success) {
    return false;
  }

  auto decodedChar = decodeRaw<char>(length);

  if (decodedChar == nullptr) {
    return false;
  }

  std::string decoded(decodedChar, length);
  string.swap(decoded);
  return true;
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

bool Message::addAttachment(const Attachment& attachment) {
  if (!attachment.isValid()) {
    return false;
  }

  _attachments.emplace_back(attachment);
  return true;
}

bool Message::setAttachments(std::vector<Attachment>&& attachments) {
  for (const auto& attachment : attachments) {
    if (!attachment.isValid()) {
      return false;
    }
  }

  _attachments = std::move(attachments);
  return true;
}

#if RL_DISABLE_XPC
const Message::Attachment::Handle MessageAttachmentHandleNull = nullptr;
#else
const Message::Attachment::Handle MessageAttachmentHandleNull = -1;
#endif

}  // namespace core
}  // namespace rl