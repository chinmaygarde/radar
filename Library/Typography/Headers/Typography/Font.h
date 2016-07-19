// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_TYPOGRAPHY_FONT_H_
#define RADAR_TYPOGRAPHY_FONT_H_

#include <Core/Macros.h>

namespace rl {
namespace type {

class Font {
 public:
  Font();

  ~Font();

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(Font);
};

}  // namespace type
}  // namespace rl

#endif  // RADAR_TYPOGRAPHY_FONT_H_
