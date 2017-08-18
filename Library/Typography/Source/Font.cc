/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Typography/Font.h>
#include <Typography/FontFace.h>

namespace rl {
namespace type {

Font::Font() = default;

Font::Font(const FontFace& fontFace, double size) {
  if (size <= 0.0 || !fontFace.isValid()) {
    return;
  }

  HBRef<hb_font_t> font(hb_font_create(fontFace.handle()), hb_font_destroy);

  if (font == nullptr) {
    return;
  }

  hb_font_set_scale(font.get(), size, size);

  hb_ft_font_set_funcs(font.get());

  hb_font_make_immutable(font.get());

  _handle = std::move(font);
}

Font::Font(Font&& other) = default;

Font& Font::operator=(Font&& other) = default;

Font::~Font() = default;

bool Font::isValid() const {
  return _handle != nullptr;
}

std::string Font::postscriptName() const {
  if (_handle == nullptr) {
    return "";
  }

  const char* name = FT_Get_Postscript_Name(hb_ft_font_get_face(_handle.get()));

  if (name == nullptr) {
    return "";
  }

  return name;
}

double Font::size() const {
  int xScale = 0;
  if (_handle != nullptr) {
    hb_font_get_scale(_handle.get(), &xScale, nullptr);
  }
  return xScale;
}

hb_font_t* Font::handle() const {
  return _handle.get();
}

}  // namespace type
}  // namespace rl
