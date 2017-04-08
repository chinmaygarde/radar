/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "HBBuffer.h"
#include <algorithm>

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

size_t HBBuffer::iterateGlyphs(GlyphIterator iterator) {
  if (iterator == nullptr || _buffer == nullptr) {
    return 0;
  }

  auto length = hb_buffer_get_length(_buffer);

  unsigned int count = 0;

  auto info = hb_buffer_get_glyph_infos(_buffer, &count);
  length = std::min(length, count);

  count = 0;

  auto position = hb_buffer_get_glyph_positions(_buffer, &count);
  length = std::min(length, count);

  size_t result = 0;

  for (size_t i = 0; i < count; i++) {
    result++;

    Coordinate advance(position[i].x_advance / 64.0,  //
                       position[i].y_advance / 64.0   //
                       );

    Coordinate offset(position[i].x_offset / 64.0,  //
                      position[i].y_offset / 64.0   //
                      );

    GlyphInfo glyphInfo(info[i].codepoint, info[i].cluster, advance, offset);

    if (!iterator(glyphInfo)) {
      break;
    }
  }

  return result;
}

}  // namespace type
}  // namespace rl
