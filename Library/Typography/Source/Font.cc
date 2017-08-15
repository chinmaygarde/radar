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

Font::Font() = default;

Font::Font(const FontFace& fontFace, double size) {
  if (size <= 0.0 || !fontFace.isValid()) {
    return;
  }

  auto font = hb_font_create(fontFace.face());

  if (font == nullptr) {
    return;
  }

  hb_font_set_scale(font, size, size);

  hb_ft_font_set_funcs(font);

  hb_font_make_immutable(font);

  __opaque = font;
  _valid = true;
}

Font::Font(Font&& other) : __opaque(other.__opaque), _valid(other._valid) {
  other.__opaque = nullptr;
  other._valid = false;
}

Font& Font::operator=(Font&& other) {
  std::swap(__opaque, other.__opaque);
  std::swap(_valid, other._valid);
  return *this;
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

double Font::size() const {
  int xScale = 0;
  if (_valid) {
    hb_font_get_scale(_font, &xScale, nullptr);
  }
  return xScale;
}

}  // namespace type
}  // namespace rl
