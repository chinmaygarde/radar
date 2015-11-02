// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Layout/Tag.h>

namespace rl {
namespace layout {

Tag::Tag(const Symbol& marker, const Symbol& other)
    : _marker(marker), _other(other) {
}

const Symbol& Tag::marker() const {
  return _marker;
}

const Symbol& Tag::other() const {
  return _other;
}

}  // namespace layout
}  // namespace rl
