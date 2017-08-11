/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Typography/Font.h>
#include <hb-ft.h>
#include "FontFace.h"

namespace rl {
namespace type {

#define _font reinterpret_cast<hb_font_t*>(__opaque)

Font::Font(const FontFace& fontFace) {
  if (!fontFace.isValid()) {
    return;
  }

  auto font = hb_font_create(fontFace.face());

  if (font == nullptr) {
    return;
  }

  hb_ft_font_set_funcs(font);

  __opaque = font;
  _valid = true;
}

Font::~Font() {
  if (_font != nullptr) {
    hb_font_destroy(_font);
  }
}

bool Font::isValid() const {
  return _valid;
}

std::string Font::postscriptName() const {
  if (!_valid) {
    return "";
  }

  const char* name = FT_Get_Postscript_Name(hb_ft_font_get_face(_font));

  if (name == nullptr) {
    return "";
  }

  return name;
}

}  // namespace type
}  // namespace rl
