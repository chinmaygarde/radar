/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/FileMapping.h>
#include <memory>
#include "FontFace.h"

namespace rl {
namespace type {

static HBBlobPtr CreateFontFileBlob(const core::URI& uri) {
  auto mapping = std::make_unique<core::FileMapping>(uri);
  if (mapping->size() == 0) {
    return {nullptr, hb_blob_destroy};
  }

  static hb_destroy_func_t onBlobDelete = [](void* userData) {
    delete reinterpret_cast<core::FileMapping*>(userData);
  };

  return {
      hb_blob_create(reinterpret_cast<const char*>(mapping->mapping()),  // data
                     mapping->size(),          // length
                     HB_MEMORY_MODE_READONLY,  // memory mode
                     mapping.release(),        // user data
                     onBlobDelete              // destroy func
                     ),
      hb_blob_destroy};
}

FontFace::FontFace(const core::URI& uri, size_t index)
    : _face(nullptr, hb_face_destroy) {
  auto blob = CreateFontFileBlob(uri);

  if (blob == nullptr) {
    return;
  }

  HBFacePtr face(hb_face_create(blob.get(), index), hb_face_destroy);

  if (face == nullptr) {
    return;
  }

  _face = std::move(face);
  _valid = true;
}

FontFace::~FontFace() = default;

bool FontFace::isValid() const {
  return _valid;
}

hb_face_t* FontFace::face() const {
  return _face.get();
}

size_t FontFace::glyphCount() const {
  return _valid ? hb_face_get_glyph_count(_face.get()) : 0;
}

}  // namespace type
}  // namespace rl
