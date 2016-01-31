// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/ArchiveVector.h>

#include <sstream>

namespace rl {
namespace core {

const ArchiveDef ArchiveVector::ArchiveDefinition = {
    .superClass = nullptr,
    .className = "Meta_Vector",
    .autoAssignName = true,
    .members = {0},
};

ArchiveSerializable::ArchiveName ArchiveVector::archiveName() const {
  return ArchiveNameAuto;
}

bool ArchiveVector::serialize(ArchiveItem& item) const {
  std::stringstream stream;
  for (const auto& key : _keys) {
    stream << key << ",";
  }
  return item.encode(0, stream.str());
}

bool ArchiveVector::deserialize(ArchiveItem& item, Namespace* ns) {
  std::string flattened;
  if (!item.decode(0, flattened)) {
    return false;
  }

  std::stringstream stream(flattened);
  int64_t single = 0;
  while (stream >> single) {
    _keys.emplace_back(single);
    stream.ignore();
  }

  return true;
}

ArchiveVector::ArchiveVector(std::vector<int64_t>&& keys)
    : _keys(std::move(keys)) {}

}  // namespace core
}  // namespace rl