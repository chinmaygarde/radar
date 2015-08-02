// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE__MESSAGE__
#define __RADARLOVE__MESSAGE__

#include "Core/Channel.h"

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
  explicit Message(const uint8_t* buffer, size_t bufferLength);

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
  bool encode(T value);

  /**
   *  Decode the given value from the message
   *
   *  @param value the valude decoded on success
   *
   *  @return if the value was successfully decoded
   */
  template <typename T>
  bool decode(T& value);

#pragma mark - Message Information Accessors

  /**
   *  A pointer to the underlying buffer of the message
   *
   *  @return the buffer pointer
   */
  const uint8_t* data() const;

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

 private:
  uint8_t* _buffer;
  size_t _bufferLength;
  size_t _dataLength;
  size_t _sizeRead;

  bool resizeBuffer(size_t size);

  DISALLOW_COPY_AND_ASSIGN(Message);
};
}

#endif /* defined(__RADARLOVE__MESSAGE__) */
