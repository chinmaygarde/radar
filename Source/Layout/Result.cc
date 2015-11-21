// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Layout/Result.h>

namespace rl {
namespace layout {

Result::Result(Type type) : _type(type) {
}

bool Result::isError() const {
  return _type != Type::Success;
}

Result::Type Result::type() const {
  return _type;
}

}  // namespace layout
}  // namespace rl
