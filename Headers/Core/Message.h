// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE__MESSAGE__
#define __RADARLOVE__MESSAGE__

#include "Core/Channel.h"
#include "Core/Attachment.h"

#include <vector>
#include <utility>
#include <memory>
#include <string.h>

namespace rl {

class Message {
 public:
  explicit Message(size_t reservedLength = 0);
  explicit Message(const uint8_t* buffer, size_t bufferLength);

  ~Message();

  bool reserve(size_t length);

  /*
   *  Encoding and decoding common types
   */

  template <typename Type>
  bool encode(Type value) {
    bool success = reserve(_dataLength + sizeof(Type));

    if (success) {
      memcpy(_buffer + _dataLength, &value, sizeof(Type));
      _dataLength += sizeof(Type);
    }

    return success;
  }

  template <typename Type>
  bool decode(Type& value) {
    if ((sizeof(Type) + _sizeRead) > _bufferLength) {
      return false;
    }

    memcpy(&value, _buffer + _sizeRead, sizeof(Type));

    _sizeRead += sizeof(Type);

    return true;
  }

  /*
   *  Encoding and decoding special attachments
   */

  void addAttachment(Attachment attachment);

  typedef std::vector<Attachment>::const_iterator AttachmentIterator;
  typedef std::pair<AttachmentIterator, AttachmentIterator> AttachmentRange;

  AttachmentRange attachmentRange() const {
    return std::make_pair(_attachments.begin(), _attachments.end());
  }

  const uint8_t* data() const { return _buffer; }

  size_t size() const { return _dataLength; }

  size_t sizeRead() const { return _sizeRead; }

 private:
  bool resizeBuffer(size_t size);

  uint8_t* _buffer;

  size_t _bufferLength;
  size_t _dataLength;
  size_t _sizeRead;

  std::vector<Attachment> _attachments;

  DISALLOW_COPY_AND_ASSIGN(Message);
};
}

#endif /* defined(__RADARLOVE__MESSAGE__) */
