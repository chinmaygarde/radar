// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_COMPOSITOR_TRANSFERRECORD_H__
#define __RADARLOVE_COMPOSITOR_TRANSFERRECORD_H__

#include <Interface/Entity.h>
#include <Geometry/Geometry.h>
#include <Compositor/Color.h>

namespace rl {

struct TransferRecord {
  Entity::Identifier identifier;
  Entity::Property property;

  union DataType {
    Entity::Identifier identifier;
    double number;
    Point point;
    Size size;
    Rect rect;
    Matrix matrix;
    Color color;

    template <typename T>
    DataType(T value) {
      static_assert(sizeof(T) <= sizeof(DataType),
                    "Size must not be larger than the available payload types");
      memcpy(this, &value, sizeof(T));
    }
  } data;

  template <typename T>
  static bool EmplaceInMessage(Message& message,
                               Entity::Identifier identity,
                               Entity::Property prop,
                               const T& varData) {
    TransferRecord* allocation = reinterpret_cast<TransferRecord*>(
        message.encodeRawUnsafe(sizeof(TransferRecord)));

    if (allocation == nullptr) {
      return false;
    }

    allocation->identifier = identity;
    allocation->property = prop;
    memcpy(&allocation->data, &varData, sizeof(T));

    return true;
  }

  static TransferRecord& NextInMessage(Message& message) {
    auto allocation = message.decodeRawUnsafe(sizeof(TransferRecord));
    RL_ASSERT(allocation != nullptr);
    return *reinterpret_cast<TransferRecord*>(allocation);
  }

  template <typename T>
  const T& transferData() const;
};

static_assert(std::is_trivially_copyable<TransferRecord>::value, "");

}  // namespace rl

#endif  // __RADARLOVE_COMPOSITOR_TRANSFERRECORD_H__
