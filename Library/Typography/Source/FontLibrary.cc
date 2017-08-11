/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Typography/Font.h>
#include <Typography/FontLibrary.h>
#include "FontFace.h"

namespace rl {
namespace type {

FontLibrary::FontLibrary() {}

FontLibrary::~FontLibrary() {}

bool FontLibrary::registerFont(const core::URI& fontFileName, size_t index) {
  /*
   *  Create a face for the font file at the URI.
   */
  auto face = std::make_unique<FontFace>(fontFileName, index);
  if (!face->isValid()) {
    return false;
  }

  /*
   *  Create a font to query the postscript name.
   */
  Font font(*face);

  if (!font.isValid()) {
    return false;
  }

  std::string fontName = font.postscriptName();

  if (fontName.size() == 0) {
    return false;
  }

  if (_registeredFonts.find(fontName) != _registeredFonts.end()) {
    /*
     *  There is already a font registration for that postscript name. Bail.
     */
    return false;
  }

  _registeredFonts[fontName] = std::move(face);
  return true;
}

size_t FontLibrary::registeredFonts() const {
  return _registeredFonts.size();
}

}  // namespace type
}  // namespace rl
