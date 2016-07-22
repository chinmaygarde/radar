// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_CORE_MESSAGE_
#define RADARLOVE_CORE_MESSAGE_

#include <Core/Config.h>
#include <Core/Macros.h>
#include <Core/Attachment.h>
#include <Core/MessageSerializable.h>
#include <Core/RawAttachment.h>

#include <string.h>

#include <memory>
#include <utility>
#include <vector>

namespace rl {
namespace core {

class MachPayload;
class SocketChannel;

class Message {
 public:
  /**
   *  Create an uninitialized message with the given reserved length.
   */
  explicit Message();

  /**
   *  Create a message with pre-initialized message data
   *
   *  @param buffer       the message data
   *  @param bufferLength the message data length
   */
  explicit Message(uint8_t* buffer, size_t bufferLength);

  /**
   *  Create a message that is already allocated for you by the platform
   *
   *  @param buffer       the message data
   *  @param bufferLength the message data length
   *  @param vmAllocated  the memory is pre-allocated by the platform allocator
   */
  explicit Message(uint8_t* buffer, size_t bufferLength, bool vmAllocated);

  /**
   *  Moves a given message
   */
  Message(Message&& message);

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
  RL_WARN_UNUSED_RESULT
  bool reserve(size_t length);

  /**
   *  Encode the given value in the message
   *
   *  @param value the value to encode
   *
   *  @return if the encode operation was successful
   */
  template <typename T, typename = only_if<rl_trivially_copyable(T)>>
  RL_WARN_UNUSED_RESULT bool encode(const T& value) {
    if (auto buffer = encodeRawUnsafe(sizeof(T))) {
      memcpy(buffer, &value, sizeof(T));
      return true;
    }
    return false;
  }

  RL_WARN_UNUSED_RESULT
  bool encode(const MessageSerializable& value);

  RL_WARN_UNUSED_RESULT
  bool encode(const AttachmentRef& attachment);

  RL_WARN_UNUSED_RESULT
  bool encode(const std::string& value);

  template <typename T, typename = only_if<rl_trivially_copyable(T)>>
  RL_WARN_UNUSED_RESULT bool encodeVectorCopyable(const std::vector<T>& vec) {
    MessageSerializable::EncodedSize count = vec.size();
    RL_RETURN_IF_FALSE(encode(count));
    for (const auto& item : vec) {
      RL_RETURN_IF_FALSE(encode(item));
    }
    return true;
  }

  template <typename T,
            typename = only_if<std::is_base_of<MessageSerializable, T>::value>>
  RL_WARN_UNUSED_RESULT bool encode(const std::vector<T>& vec) {
    MessageSerializable::EncodedSize count = vec.size();
    RL_RETURN_IF_FALSE(encode(count))
    for (const auto& item : vec) {
      RL_RETURN_IF_FALSE(item.serialize(*this))
    }
    return true;
  }

  template <typename T>
  RL_WARN_UNUSED_RESULT T* encodeRaw(size_t size) {
    return reinterpret_cast<T*>(
        alignAllocation(encodeRawUnsafe(size), alignof(T), true));
  }

  RL_WARN_UNUSED_RESULT
  size_t encodeOffsetRawUnsafe(size_t size);

  /**
   *  Decode the given value from the message
   *
   *  @param value the valude decoded on success
   *
   *  @return if the value was successfully decoded
   */
  template <typename T, typename = only_if<rl_trivially_copyable(T)>>
  RL_WARN_UNUSED_RESULT bool decode(T& value, core::Namespace*) {
    if (auto buffer = decodeRawUnsafe(sizeof(T))) {
      memcpy(&value, buffer, sizeof(T));
      return true;
    }
    return false;
  }

  RL_WARN_UNUSED_RESULT
  bool decode(MessageSerializable& value, Namespace* ns);

  RL_WARN_UNUSED_RESULT
  bool decode(RawAttachment& attachment);

  RL_WARN_UNUSED_RESULT
  bool decode(std::string& string);

  template <typename T,
            typename = only_if<rl_trivially_copyable(T) &&
                               std::is_default_constructible<T>::value>>
  RL_WARN_UNUSED_RESULT bool decodeVectorCopyable(std::vector<T>& vec,
                                                  Namespace* ns) {
    MessageSerializable::EncodedSize count = 0;
    RL_RETURN_IF_FALSE(decode(count, ns));
    for (MessageSerializable::EncodedSize i = 0; i < count; i++) {
      vec.emplace_back();
      RL_RETURN_IF_FALSE(decode(vec.back(), ns));
    }
    return true;
  }

  template <typename T,
            typename = only_if<std::is_base_of<MessageSerializable, T>::value &&
                               std::is_default_constructible<T>::value>>
  RL_WARN_UNUSED_RESULT bool decode(std::vector<T>& vec, Namespace* ns) {
    MessageSerializable::EncodedSize count = 0;
    RL_RETURN_IF_FALSE(decode(count, ns));
    for (MessageSerializable::EncodedSize i = 0; i < count; i++) {
      vec.emplace_back();
      RL_RETURN_IF_FALSE(vec.back().deserialize(*this, ns));
    }
    return true;
  }

  template <typename T>
  RL_WARN_UNUSED_RESULT T* decodeRaw(size_t size) {
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
   *  message may be read again. Only messages that do not contain attachments
   *  can be rewound for reads. This is because attachment lifecycle is managed
   *  by the callers of the inital decode.
   */
  RL_WARN_UNUSED_RESULT
  bool rewindRead();

 private:
  uint8_t* _buffer;
  std::vector<AttachmentRef> _attachments;
  size_t _bufferLength;
  size_t _dataLength;
  size_t _sizeRead;
  size_t _attachmentsRead;
  bool _vmAllocated;

  bool resizeBuffer(size_t size);
  uint8_t* encodeRawUnsafe(size_t size);
  uint8_t* decodeRawUnsafe(size_t size);
  uint8_t* alignAllocation(uint8_t* allocation,
                           size_t alignment,
                           bool encoding);

  friend class MachPayload;
  friend class SocketChannel;

  size_t attachmentsSize() const;
  const std::vector<AttachmentRef>& attachments() const;

  RL_DISALLOW_COPY_AND_ASSIGN(Message);
};

using Messages = std::vector<Message>;

}  // namespace core
}  // namespace rl

#endif  // RADARLOVE_CORE_MESSAGE_
