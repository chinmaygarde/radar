// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_CORE_SERIALIZABLE_H__
#define __RADARLOVE_CORE_SERIALIZABLE_H__

#include <Core/Message.h>
#include <Core/Utilities.h>

namespace rl {

class Serializable {
  virtual bool serialize(Message& message) const = 0;
  virtual bool deserialize(Message& message) = 0;

 public:
  using VectorSize = size_t;

  template <typename T,
            typename = rl::enable_if_t<std::is_base_of<Serializable, T>::value>>
  static bool SerializeVector(const std::vector<T>& vector, Message& message) {
    VectorSize count = vector.size();
    auto result = message.encode(count);
    for (const auto& item : vector) {
      result &= item.serialize(message);
    }
    return result;
  }

  template <typename T,
            typename = rl::enable_if_t<std::is_base_of<Serializable, T>::value>,
            typename = rl::enable_if_t<std::is_default_constructible<T>::value>>
  static bool DeserializeVector(std::vector<T>& vector, Message& message) {
    VectorSize count = 0;
    auto result = message.decode(count);
    for (auto i = 0; i < count; i++) {
      vector.emplace_back();
      result &= vector.back().deserialize(message);
    }
    return result;
  }
};

}  // namespace rl

#endif  // __RADARLOVE_CORE_SERIALIZABLE_H__
