// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_COMPOSITOR_TRANSFERRECORD_H_
#define RADARLOVE_COMPOSITOR_TRANSFERRECORD_H_

#include <Interface/Entity.h>
#include <Geometry/Geometry.h>
#include <Coordinator/Color.h>
#include <Core/Utilities.h>

#include <stddef.h>

namespace rl {
namespace coordinator {

struct TransferRecord {
  interface::Entity::Identifier identifier;
  interface::Entity::Property property;

  union DataType {
    interface::Entity::Identifier identifier;
    double number;
    geom::Point point;
    geom::Size size;
    geom::Rect rect;
    geom::Matrix matrix;
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
  static bool EmplaceInMessage(core::Message& message,
                               interface::Entity::Identifier identity,
                               interface::Entity::Property prop,
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

    auto allocation = message.encodeRaw<TransferRecord>(recordSize);

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

  static TransferRecord& NextInMessage(core::Message& message) {
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
    auto allocation = message.decodeRaw<TransferRecord>(recordSize);
    RL_ASSERT(allocation != nullptr);
    return *allocation;
  }

  template <typename T>
  const T& transferData() const;
};

static_assert(rl_trivially_copyable(TransferRecord), "");

}  // namespace coordinator
}  // namespace rl

#endif  // RADARLOVE_COMPOSITOR_TRANSFERRECORD_H_
