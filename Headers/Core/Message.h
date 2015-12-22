// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_CORE_MESSAGE_
#define RADARLOVE_CORE_MESSAGE_

#include <Core/Macros.h>
#include <Core/Serializable.h>
#include <Core/Utilities.h>

#include <string.h>

#include <memory>
#include <utility>
#include <vector>

namespace rl {
namespace core {

class Message {
 public:
  class Attachment {
   public:
    using Handle = uintptr_t;

    Attachment(Handle handle = 0);

    Attachment(Attachment&& other);

    bool isValid() const;

    Handle handle() const;

   private:
    Handle _handle;

    RL_DISALLOW_COPY_AND_ASSIGN(Attachment);
  };

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
   *  Create a message that is already allocated for you by the the platform
   *
   *  @param buffer       the message data
   *  @param bufferLength the message data length
   *  @param vmDeallocate must be true, use the other ctor otherwise
   */
  explicit Message(uint8_t* buffer, size_t bufferLength, bool vmDeallocate);

  /**
   *  Moves a given message
   */
  Message(Message&& message);

  /**
   *  Construct a message with the given attachment
   */
  Message(Attachment&& attachment);

  /**
   *  Collect the message
   */
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

  /**
   *  Encode the given value in the message
   *
   *  @param value the value to encode
   *
   *  @return if the encode operation was successful
   */
  template <typename T, typename = only_if<rl_trivially_copyable(T)>>
  bool encode(const T& value) {
    if (auto buffer = encodeRawUnsafe(sizeof(T))) {
      memcpy(buffer, &value, sizeof(T));
      return true;
    }
    return false;
  }

  bool encode(const Serializable& value);

  template <typename T,
            typename = only_if<std::is_base_of<Serializable, T>::value>>
  bool encode(const std::vector<T>& vec) {
    Serializable::EncodedSize count = vec.size();
    auto result = encode(count);
    for (const auto& item : vec) {
      result &= item.serialize(*this);
    }
    return result;
  }

  template <typename T>
  T* encodeRaw(size_t size) {
    return reinterpret_cast<T*>(
        alignAllocation(encodeRawUnsafe(size), alignof(T), true));
  }

  size_t encodeOffsetRawUnsafe(size_t size);

  /**
   *  Decode the given value from the message
   *
   *  @param value the valude decoded on success
   *
   *  @return if the value was successfully decoded
   */
  template <typename T, typename = only_if<rl_trivially_copyable(T)>>
  bool decode(T& value) {
    if (auto buffer = decodeRawUnsafe(sizeof(T))) {
      memcpy(&value, buffer, sizeof(T));
      return true;
    }
    return false;
  }

  bool decode(Serializable& value);

  template <typename T,
            typename = only_if<std::is_base_of<Serializable, T>::value ||
                               std::is_default_constructible<T>::value>>
  bool decode(std::vector<T>& vec) {
    Serializable::EncodedSize count = 0;
    auto result = decode(count);
    for (Serializable::EncodedSize i = 0; i < count; i++) {
      vec.emplace_back();
      result &= vec.back().deserialize(*this);
    }
    return result;
  }

  template <typename T>
  T* decodeRaw(size_t size) {
    return reinterpret_cast<T*>(
        alignAllocation(decodeRawUnsafe(size), alignof(T), false));
  }

  uint8_t* operator[](size_t index);

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

  /**
   *  @return if the entire data buffer has been read
   */
  bool readCompleted() const;

  /**
   *  Reset the read cursor used for reading. This makes it so that the same
   *  message may be read again.
   */
  void rewindRead();

  /**
   *  @return if the message in its current configuration represents a valid
   *          message that is suitable to be sent down a channel
   */
  bool isValid() const;

  /**
   *  An optional message attachment
   *
   *  @return the message attachment
   */
  const Attachment& attachment() const;

 private:
  uint8_t* _buffer;
  Attachment _attachment;
  size_t _bufferLength;
  size_t _dataLength;
  size_t _sizeRead;
  bool _vmDeallocate = false;

  bool resizeBuffer(size_t size);

  uint8_t* encodeRawUnsafe(size_t size);

  uint8_t* decodeRawUnsafe(size_t size);

  uint8_t* alignAllocation(uint8_t* allocation,
                           size_t alignment,
                           bool encoding);

  RL_DISALLOW_COPY_AND_ASSIGN(Message);
};

using Messages = std::vector<Message>;

}  // namespace core
}  // namespace rl

#endif  // RADARLOVE_CORE_MESSAGE_
