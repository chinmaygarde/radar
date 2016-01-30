// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Archive.h>

namespace rl {
namespace core {

class ArchiveClassRegistration {
 public:
  ArchiveClassRegistration(ArchiveDef definition);

  using ColumnResult = std::pair<size_t, bool>;

  ColumnResult findColumn(const std::string& className,
                          ArchiveSerializable::Member member) const;

  const std::string& tableName() const;

  size_t columnCount() const;

 private:
  using MemberColumnMap = std::map<ArchiveSerializable::Member, size_t>;
  using ClassMap = std::map<std::string, MemberColumnMap>;

  ClassMap _classMap;
  std::string _tableName;
  size_t _columnCount;

  RL_DISALLOW_COPY_AND_ASSIGN(ArchiveClassRegistration);
};

}  // namespace core
}  // namespace rl
