// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_CORE_MESSAGESERIALIZABLE_H_
#define RADARLOVE_CORE_MESSAGESERIALIZABLE_H_

#include <Core/Utilities.h>

namespace rl {
namespace core {

class Message;
class Namespace;

class MessageSerializable {
 public:
  virtual bool serialize(Message& message) const = 0;

  virtual bool deserialize(Message& message, Namespace* ns) = 0;

  using EncodedSize = size_t;
};

}  // namespace core
}  // namespace rl

#endif  // RADARLOVE_CORE_MESSAGESERIALIZABLE_H_
