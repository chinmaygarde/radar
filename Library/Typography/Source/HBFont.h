// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_TYPOGRAPHY_HBFONT_H_
#define RADAR_TYPOGRAPHY_HBFONT_H_

#include <Core/Macros.h>
#include <Core/URI.h>
#include <Typography/Types.h>
#include <hb.h>
#include "FTFace.h"

namespace rl {
namespace type {

class HBFont {
 public:
  HBFont();

  HBFont(FTFaceAccess& face);

  HBFont(HBFont&&);

  ~HBFont();

  hb_font_t* handle() const;

  bool isValid() const;

  Point pointSize() const;

  void setPointSize(Point size);

 private:
  hb_font_t* _font;

  RL_DISALLOW_COPY_AND_ASSIGN(HBFont);
};

}  // namespace type
}  // namespace rl

#endif  // RADAR_TYPOGRAPHY_HBFONT_H_
