/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

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

  ArchiveStatement queryStatement(bool single) const;

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
