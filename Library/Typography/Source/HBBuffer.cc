// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "HBBuffer.h"

namespace rl {
namespace type {

HBBuffer::HBBuffer(const HBFont& font, const std::string& string)
    : _buffer(hb_buffer_create()) {
  hb_buffer_add_utf8(_buffer, string.data(), string.size(), 0, -1);
  hb_buffer_guess_segment_properties(_buffer);
  hb_shape(font.handle(), _buffer, NULL, 0);
}

size_t HBBuffer::length() const {
  return hb_buffer_get_length(_buffer);
}

HBBuffer::~HBBuffer() {
  hb_buffer_destroy(_buffer);
}

}  // namespace type
}  // namespace rl
