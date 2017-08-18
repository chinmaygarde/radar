/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Typography/Font.h>
#include <Typography/FontFace.h>
#include <Typography/FontLibrary.h>

namespace rl {
namespace type {

FontLibrary::FontLibrary() = default;

FontLibrary::~FontLibrary() = default;

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
  Font font(*face, 14.0);

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

bool FontLibrary::isValid() const {
  return _registeredFonts.size() > 0;
}

size_t FontLibrary::registeredFonts() const {
  return _registeredFonts.size();
}

Font FontLibrary::fontForDescriptor(const FontDescriptor& descriptor) const {
  if (descriptor.pointSize() <= 0.0) {
    return {};
  }

  auto found = _registeredFonts.find(descriptor.postscriptName());
  if (found == _registeredFonts.end()) {
    return {};
  }

  return Font{*(found->second.get()), descriptor.pointSize()};
}

}  // namespace type
}  // namespace rl
