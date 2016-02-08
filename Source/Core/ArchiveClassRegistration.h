// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Archive.h>
#include <map>

namespace rl {
namespace core {

class ArchiveClassRegistration {
 public:
  using ColumnResult = std::pair<size_t, bool>;
  ColumnResult findColumn(const std::string& className,
                          ArchiveSerializable::Member member) const;

  const std::string& className() const;

  size_t memberCount() const;

  bool isReady() const;

  ArchiveStatement insertStatement() const;

  ArchiveStatement queryStatement() const;

  static const size_t NameIndex = 0;

 private:
  using MemberColumnMap = std::map<ArchiveSerializable::Member, size_t>;
  using ClassMap = std::map<std::string, MemberColumnMap>;

  friend class ArchiveDatabase;

  ArchiveClassRegistration(ArchiveDatabase& database, ArchiveDef definition);

  bool createTable(bool autoIncrement);

  ArchiveDatabase& _database;
  ClassMap _classMap;
  std::string _className;
  size_t _memberCount;
  bool _isReady;

  RL_DISALLOW_COPY_AND_ASSIGN(ArchiveClassRegistration);
};

}  // namespace core
}  // namespace rl
