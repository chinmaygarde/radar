/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <unicode/unistr.h>
#include "ShapedTextRun.h"

namespace rl {
namespace type {

static hb_direction_t ToHBDirection(TextRun::Direction direction) {
  switch (direction) {
    case TextRun::Direction::LeftToRight:
      return HB_DIRECTION_LTR;
    case TextRun::Direction::RightToLeft:
      return HB_DIRECTION_RTL;
    default:
      return HB_DIRECTION_INVALID;
  }
  return HB_DIRECTION_INVALID;
}

ShapedTextRun::ShapedTextRun() = default;

ShapedTextRun::ShapedTextRun(const String& string,
                             const TextRun& run,
                             const FontLibrary& library) {
  if (!TextRange{0, string.size()}.containsRange(run.range())) {
    return;
  }

  /*
   *  Resolve the font. This is what we are going to use the shape the buffer.
   */
  _font = library.fontForDescriptor(run.descriptor());

  if (!_font.isValid()) {
    return;
  }

  /*
   *  Create the buffer to shape.
   */
  _buffer = {hb_buffer_create(), hb_buffer_destroy};

  if (_buffer == nullptr) {
    return;
  }

  /*
   *  Populate the buffer to shape.
   */
  hb_buffer_add_utf16(_buffer.get(),  // buffer
                      reinterpret_cast<const uint16_t*>(
                          string.unicodeString().getBuffer()),  // text
                      string.unicodeString().length(),          // text length
                      run.range().start,                        // item offset
                      run.range().length                        // item length
  );

  /*
   *  Set the buffer direction. We already detected this when we setup runs.
   */
  hb_buffer_set_direction(_buffer.get(), ToHBDirection(run.direction()));

  /*
   *  TODO: Set the script of the buffer.
   */

  /*
   *  TODO: Set the language of the buffer.
   */

  /*
   *  This is a fallback.
   */
  hb_buffer_guess_segment_properties(_buffer.get());

  /*
   *  Finally, shape the thing!
   */
  hb_shape(reinterpret_cast<hb_font_t*>(_font.handle()),  // font
           _buffer.get(),                                 // buffer
           nullptr,                                       // features
           0                                              // features count
  );

  /*
   *  After successful shaping, the buffer will contain glyphs.
   */
  _valid = hb_buffer_get_content_type(_buffer.get()) ==
           HB_BUFFER_CONTENT_TYPE_GLYPHS;
}

ShapedTextRun::ShapedTextRun(ShapedTextRun&& o)
    : _buffer(std::move(o._buffer)),
      _font(std::move(o._font)),
      _valid(o._valid) {
  o._valid = false;
}

ShapedTextRun::~ShapedTextRun() = default;

bool ShapedTextRun::isValid() const {
  return _valid;
}

geom::Size ShapedTextRun::size() const {
  if (!_valid) {
    return {};
  }

  uint32_t length = 0;
  hb_glyph_position_t* position =
      hb_buffer_get_glyph_positions(_buffer.get(), &length);

  geom::Size size;

  for (uint32_t i = 0; i < length; i++) {
    size.width += position[i].x_advance;
    size.height += position[i].y_advance;
  }

  return size;
}

}  // namespace type
}  // namespace rl
