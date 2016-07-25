// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "HBFont.h"

namespace rl {
namespace type {

HBFont::HBFont() : _font(nullptr) {}

HBFont::HBFont(const FTFaceAccess& face) : _font(face.createHBFont()) {}

HBFont::HBFont(HBFont&& o) : _font(o._font) {
  o._font = nullptr;
}

HBFont::~HBFont() {
  hb_font_destroy(_font);
}

hb_font_t* HBFont::handle() const {
  return _font;
}

bool HBFont::isValid() const {
  return _font != nullptr;
}

}  // namespace type
}  // namespace rl
