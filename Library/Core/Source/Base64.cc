// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Base64.h>
#include <base64/base64.h>

namespace rl {
namespace core {

Allocation Base64Decode(const uint8_t* data) {
  auto length = ::Base64decode_len(reinterpret_cast<const char*>(data));

  if (length <= 0) {
    return {};
  }

  Allocation decoded;

  if (!decoded.resize(length)) {
    return {};
  }

  auto newLength = ::Base64decode(reinterpret_cast<char*>(decoded.data()),
                                  reinterpret_cast<const char*>(data));

  if (newLength <= 0) {
    return {};
  }

  if (newLength != length) {
    if (!decoded.resize(newLength)) {
      return {};
    }
  }

  return decoded;
}

}  // namespace core
}  // namespace rl
