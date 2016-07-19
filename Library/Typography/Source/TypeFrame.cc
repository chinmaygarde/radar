// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Typography/TypeFrame.h>

//#include <hb.h>

namespace rl {
namespace type {

TypeFrame::TypeFrame(const std::string& string) {
  //  auto buffer = hb_buffer_create();
  //
  //
  //  hb_buffer_add_utf8(buffer, string.data(), string.size(), 0, -1);
  //
  //  hb_buffer_guess_segment_properties(buffer);
  //
  //   hb_buffer_get_length(buffer);
  //
  //  hb_shape(<#hb_font_t *font#>, <#hb_buffer_t *buffer#>, <#const
  //  hb_feature_t *features#>, <#unsigned int num_features#>)
  //
  //  hb_buffer_destroy(buffer);
}

TypeFrame::~TypeFrame() {}

}  // namespace type
}  // namespace rl
