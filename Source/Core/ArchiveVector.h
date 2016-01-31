// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef RADARLOVE_CORE_ARCHIVEVECTOR_H_
#define RADARLOVE_CORE_ARCHIVEVECTOR_H_

#include <Core/Archive.h>

namespace rl {
namespace core {

class ArchiveVector : public ArchiveSerializable {
 public:
  static const ArchiveDef ArchiveDefinition;

  ArchiveName archiveName() const override;

  bool serialize(ArchiveItem& item) const override;

  bool deserialize(ArchiveItem& item, Namespace* ns) override;

 private:
  std::vector<int64_t> _keys;

  friend class ArchiveItem;

  ArchiveVector(std::vector<int64_t>&& keys);

  RL_DISALLOW_COPY_AND_ASSIGN(ArchiveVector);
};

}  // namespace core
}  // namespace rl

#endif // RADARLOVE_CORE_ARCHIVEVECTOR_H_
