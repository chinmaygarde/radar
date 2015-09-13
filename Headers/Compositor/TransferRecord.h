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
  TransferRecord(Entity::Identifier identity,
                 Entity::Property prop,
                 const T& varData)
      : identifier(identity), property(prop), data(varData) {}

  template <typename T>
  static bool Emplaced(Message& message,
                       Entity::Identifier identity,
                       Entity::Property prop,
                       const T& varData) {
    void* allocation = message.encodeRawUnsafe(sizeof(TransferRecord));

    if (allocation == nullptr) {
      return false;
    }

    new (allocation) TransferRecord(identity, prop, varData);
    return true;
  }

  template <typename T>
  const T& transferData() const;
};

}  // namespace rl

#endif  // __RADARLOVE_COMPOSITOR_TRANSFERRECORD_H__
