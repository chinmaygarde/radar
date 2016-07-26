// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "FTFace.h"

#include <hb-ft.h>

namespace rl {
namespace type {

#define TO_FT(x) ((x)*64.0)

static const double kDefaultCharacterSize = 14.0;

FTFace::FTFace() : _handle(nullptr), _isValid(false) {}

FTFace::FTFace(FT_Library library, const core::URI& uri)
    : _handle(nullptr), _isValid(false) {
  FT_Error error =
      FT_New_Face(library,                                 // library
                  uri.filesystemRepresentation().c_str(),  // font uri
                  0,                                       // face index
                  &_handle                                 // face (out)
                  );

  if (error != 0) {
    return;
  }

  _isValid = true;

  if (auto postscriptName = FT_Get_Postscript_Name(_handle)) {
    _postscriptName = postscriptName;
  }

  FT_Set_Char_Size(_handle, TO_FT(kDefaultCharacterSize),
                   TO_FT(kDefaultCharacterSize), 0, 0);
}

FTFace::FTFace(FTFace&& o) : _handle(o._handle), _isValid(o._isValid) {
  o._handle = nullptr;
  o._isValid = false;
}

FTFace::~FTFace() {
  if (_handle != nullptr) {
    FT_Done_Face(_handle);
  }
}

bool FTFace::isValid() const {
  return _isValid;
}

const std::string& FTFace::postscriptName() const {
  return _postscriptName;
}

FTFaceAccess::FTFaceAccess(std::mutex& mutex, FTFace& face)
    : _mutex(mutex), _face(face) {
  _mutex.lock();
}

FTFaceAccess::~FTFaceAccess() {
  _mutex.unlock();
}

bool FTFaceAccess::setPointSize(Point size) {
  return FT_Set_Char_Size(_face._handle, TO_FT(size), TO_FT(size), 0, 0) == 0;
}

hb_font_t* FTFaceAccess::createHBFont() const {
  return hb_ft_font_create_referenced(_face._handle);
}

}  // namespace type
}  // namespace rl
