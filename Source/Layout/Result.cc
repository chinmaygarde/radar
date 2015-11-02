// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Layout/Result.h>

namespace rl {
namespace layout {

Result::Result(const std::string& message, bool isError)
    : _message(message), _isError(isError) {
}

const std::string& Result::message() const {
  return _message;
}

bool Result::isError() const {
  return _isError;
}

}  // namespace layout
}  // namespace rl
