// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_TYPOGRAPHY_HBBUFFER_H_
#define RADAR_TYPOGRAPHY_HBBUFFER_H_

#include <Core/Macros.h>
#include <Typography/Types.h>
#include <hb.h>
#include <string>
#include "HBFont.h"

namespace rl {
namespace type {

struct GlyphInfo {
  Codepoint codepoint;
  size_t cluster;
  Coordinate advance;
  Coordinate offset;

  GlyphInfo(Codepoint aCodepoint,
            size_t aCluster,
            Coordinate anAdvance,
            Coordinate anOffset)
      : codepoint(aCodepoint),
        cluster(aCluster),
        advance(anAdvance),
        offset(anOffset) {}
};

class HBBuffer {
 public:
  HBBuffer(const HBFont& font, const std::string& string);

  ~HBBuffer();

  size_t length() const;

  using GlyphIterator = std::function<bool(const GlyphInfo& info)>;

  size_t iterateGlyphs(GlyphIterator iterator);

 private:
  hb_buffer_t* _buffer;

  RL_DISALLOW_COPY_AND_ASSIGN(HBBuffer);
};

}  // namespace type
}  // namespace rl

#endif  // RADAR_TYPOGRAPHY_HBBUFFER_H_
