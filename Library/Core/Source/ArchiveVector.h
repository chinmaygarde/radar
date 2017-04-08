/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Archive.h>

namespace rl {
namespace core {

class ArchiveVector : public ArchiveSerializable {
 public:
  static const ArchiveDef ArchiveDefinition;

  ArchiveName archiveName() const override;

  const std::vector<int64_t> keys() const;

  bool serialize(ArchiveItem& item) const override;

  bool deserialize(ArchiveItem& item, Namespace* ns) override;

 private:
  std::vector<int64_t> _keys;

  friend class ArchiveItem;

  ArchiveVector();
  ArchiveVector(std::vector<int64_t>&& keys);

  RL_DISALLOW_COPY_AND_ASSIGN(ArchiveVector);
};

}  // namespace core
}  // namespace rl
