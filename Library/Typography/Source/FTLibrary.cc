// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "FTLibrary.h"

#include <Core/Utilities.h>

#include "FTFace.h"

namespace rl {
namespace type {

FTLibrary::FTLibrary() : _library(nullptr) {
  FT_Init_FreeType(&_library);
}

FTLibrary::~FTLibrary() {
  _registry.clear();
  FT_Done_FreeType(_library);
}

FTLibrary::MutexFacePair::MutexFacePair(FTFace aFace)
    : face(std::move(aFace)) {}

bool FTLibrary::registerFont(const core::URI& uri) {
  FTFace face(_library, uri);

  if (!face.isValid()) {
    return false;
  }

  auto postscriptName = face.postscriptName();

  if (postscriptName.size() == 0) {
    return false;
  }

  return _registry
      .emplace(std::move(postscriptName),
               core::make_unique<MutexFacePair>(std::move(face)))
      .second;
}

HBFont FTLibrary::fontForTypeface(const std::string& postscriptName) {
  auto found = _registry.find(postscriptName);

  if (found == _registry.end()) {
    return HBFont{};
  }

  auto& entry = found->second;

  FTFaceAccess face(entry->mutex, entry->face);
  return HBFont{face};
}

}  // namespace type
}  // namespace rl