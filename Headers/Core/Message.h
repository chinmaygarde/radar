// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE__MESSAGE__
#define __RADARLOVE__MESSAGE__

#include <Core/Macros.h>

#include <vector>
#include <utility>
#include <memory>
#include <string.h>

namespace rl {
class Message {
 public:
  /**
   *  Create an uninitialized message with the given reserved length.
   *
   *  @param reservedLength the reserved length. This is a hint that indicates
   *                        the initial size of the buffer used for the message.
   *                        This may be resized as needed.
   */
  explicit Message(size_t reservedLength = 0);

  /**
   *  Create a message with pre-initialized message data
   *
   *  @param buffer       the message data
   *  @param bufferLength the message data length
   */
  explicit Message(uint8_t* buffer, size_t bufferLength);

  /**
   *  Create a message that is already allocated for you by the mach kernel
   *  and must be released via a call to `vm_deallocate`
   *
   *  TODO: Need to be able to specify custom allocators and deallocators. This
   *        is BS.
   *
   *  @param buffer       the message data
   *  @param bufferLength the message data length
   *  @param vmDeallocate must be true, use the other ctor otherwise
   */
  explicit Message(uint8_t* buffer, size_t bufferLength, bool vmDeallocate);

  Message(Message&& message) noexcept = default;

  ~Message();

  /**
   *  If the total size of the encoded message is known ahead of time, this can
   *  be reserved to prevent unnecessary buffer resizes during message encoding.
   *
   *  @param length the buffer size to encode
   *
   *  @return if the resize operation was successful
   */
  bool reserve(size_t length);

#pragma mark - Encoding and decoding messages

  /**
   *  Encode the given value in the message
   *
   *  @param value the value to encode
   *
   *  @return if the encode operation was successful
   */
  template <typename T>
  bool encode(T value) {
    bool success = reserve(_dataLength + sizeof(T));

    if (success) {
      memcpy(_buffer + _dataLength, &value, sizeof(T));
      _dataLength += sizeof(T);
    }

    return success;
  }

  /**
   *  Decode the given value from the message
   *
   *  @param value the valude decoded on success
   *
   *  @return if the value was successfully decoded
   */
  template <typename T>
  bool decode(T& t) {
    if ((sizeof(T) + _sizeRead) > _bufferLength) {
      return false;
    }

    memcpy(&t, _buffer + _sizeRead, sizeof(T));

    _sizeRead += sizeof(T);

    return true;
  }

#pragma mark - Message Information Accessors

  /**
   *  A pointer to the underlying buffer of the message
   *
   *  @return the buffer pointer
   */
  uint8_t* data() const;

  /**
   *  The size of the underlying encoded data
   *
   *  @return the data size
   */
  size_t size() const;

  /**
   *  The size of the data already read during previous `decode` operations
   *
   *  @return the size of data already read
   */
  size_t sizeRead() const;

#pragma mark - Reserved Header Length

  size_t reservedHeaderSize() const;

 private:
  uint8_t* _buffer;
  size_t _bufferLength;
  size_t _dataLength;
  size_t _sizeRead;
  bool _vmDeallocate = false;

  bool resizeBuffer(size_t size);

  DISALLOW_COPY_AND_ASSIGN(Message);
};

using Messages = std::vector<std::unique_ptr<Message>>;

class Serializable {
  virtual void serialize(Message& message) const = 0;
  virtual void deserialize(Message& message) = 0;
};
}

#endif /* defined(__RADARLOVE__MESSAGE__) */
