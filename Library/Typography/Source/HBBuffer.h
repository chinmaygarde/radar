// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_TYPOGRAPHY_HBBUFFER_H_
#define RADAR_TYPOGRAPHY_HBBUFFER_H_

#include <Core/Macros.h>

#include <hb.h>
#include <string>

#include "HBFont.h"

namespace rl {
namespace type {

class HBBuffer {
 public:
  HBBuffer(const HBFont& font, const std::string& string);

  ~HBBuffer();

  size_t length() const;

 private:
  hb_buffer_t* _buffer;

  RL_DISALLOW_COPY_AND_ASSIGN(HBBuffer);
};

}  // namespace type
}  // namespace rl

#endif  // RADAR_TYPOGRAPHY_HBBUFFER_H_
