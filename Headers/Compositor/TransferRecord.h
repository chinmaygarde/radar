// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_COMPOSITOR_TRANSFERRECORD_H_
#define RADARLOVE_COMPOSITOR_TRANSFERRECORD_H_

#include <Interface/Entity.h>
#include <Geometry/Geometry.h>
#include <Compositor/Color.h>
#include <Core/Utilities.h>

#include <stddef.h>

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

  using TransferRecordDataSize = size_t;

  static constexpr size_t TransferRecordInvariantsSize() {
    static_assert(
        offsetof(TransferRecord, data) + sizeof(data) == sizeof(TransferRecord),
        "The data member must be the last item in the struct");
    return offsetof(TransferRecord, data);
  }

  template <typename T>
  static bool EmplaceInMessage(Message& message,
                               Entity::Identifier identity,
                               Entity::Property prop,
                               const T& varData) {
    /*
     *  Write the size of the 'data' field contained within this transfer record
     */
    TransferRecordDataSize size = sizeof(T);
    if (!message.encode(size)) {
      return false;
    }

    /*
     *  Allocate just enough space within the message for the transfer record
     *  invariants and the data. The 'data' field is the last item to appear
     *  within the transfer record
     */
    const auto recordSize = TransferRecordInvariantsSize() + sizeof(T);

    TransferRecord* allocation =
        reinterpret_cast<TransferRecord*>(message.encodeRawUnsafe(recordSize));

    if (allocation == nullptr) {
      return false;
    }

    /*
     *  Essentially a placement new (but not really because of the union at the
     *  end)
     */
    allocation->identifier = identity;
    allocation->property = prop;
    memcpy(&allocation->data, &varData, sizeof(T));

    return true;
  }

  static TransferRecord& NextInMessage(Message& message) {
    /*
     *  Decode the size of the 'data' field
     */
    TransferRecordDataSize size = 0;
    bool result = message.decode(size);
    RL_ASSERT(result && size > 0);

    /*
     *  The transfer record size is the size of the invariants plus the size of
     *  the data
     */
    const auto recordSize = TransferRecordInvariantsSize() + size;
    auto allocation = message.decodeRawUnsafe(recordSize);
    RL_ASSERT(allocation != nullptr);
    return *reinterpret_cast<TransferRecord*>(allocation);
  }

  template <typename T>
  const T& transferData() const;
};

static_assert(rl_trivially_copyable(TransferRecord), "");

}  // namespace rl

#endif  // RADARLOVE_COMPOSITOR_TRANSFERRECORD_H_
