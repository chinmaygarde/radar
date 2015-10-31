// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/TransferRecord.h>

namespace rl {

template <>
const interface::Entity::Identifier&
TransferRecord::transferData<interface::Entity::Identifier>() const {
  return data.identifier;
}

template <>
const double& TransferRecord::transferData<double>() const {
  return data.number;
}

template <>
const geom::Point& TransferRecord::transferData<geom::Point>() const {
  return data.point;
}

template <>
const geom::Size& TransferRecord::transferData<geom::Size>() const {
  return data.size;
}

template <>
const geom::Rect& TransferRecord::transferData<geom::Rect>() const {
  return data.rect;
}

template <>
const geom::Matrix& TransferRecord::transferData<geom::Matrix>() const {
  return data.matrix;
}

template <>
const Color& TransferRecord::transferData<Color>() const {
  return data.color;
}

}  // namespace rl
