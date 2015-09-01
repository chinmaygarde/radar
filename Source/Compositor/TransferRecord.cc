// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/TransferRecord.h>

namespace rl {

template <>
const Entity::Identifier& TransferRecord::transferData<Entity::Identifier>()
    const {
  return data.identifier;
}

template <>
const double& TransferRecord::transferData<double>() const {
  return data.number;
}

template <>
const Point& TransferRecord::transferData<Point>() const {
  return data.point;
}

template <>
const Size& TransferRecord::transferData<Size>() const {
  return data.size;
}

template <>
const Rect& TransferRecord::transferData<Rect>() const {
  return data.rect;
}

template <>
const Matrix& TransferRecord::transferData<Matrix>() const {
  return data.matrix;
}

template <>
const Color& TransferRecord::transferData<Color>() const {
  return data.color;
}

}  // namespace rl
