// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_CORE_ARCHIVE_H_
#define RADARLOVE_CORE_ARCHIVE_H_

#include <Core/Allocation.h>
#include <Core/Macros.h>
#include <Core/Utilities.h>

#include <map>
#include <string>
#include <vector>

namespace rl {
namespace core {

class ArchiveItem;

class ArchiveSerializable {
 public:
  using Member = uint64_t;
  using Members = std::vector<Member>;
  using ArchiveName = uint64_t;

  virtual ArchiveName archiveName() const = 0;

  virtual bool serialize(ArchiveItem& item) const = 0;
  virtual bool deserialize(ArchiveItem& item) = 0;
};

template <class T>
class ArchiveDef {
 public:
  std::string className();
  ArchiveSerializable::Members members();
};

class Archive {
 public:
  Archive(const std::string& path);

  ~Archive();

  bool isReady() const;

  template <class T>
  bool registerDefinition() {
    ArchiveDef<T> definition;
    return registerClass(definition.className(), definition.members());
  }

  template <class T,
            class = only_if<std::is_base_of<ArchiveSerializable, T>::value>>
  bool archive(const T& archivable) {
    ArchiveDef<T> definition;
    return archiveInstance(definition.className(), archivable);
  }

  template <class T,
            class = only_if<std::is_base_of<ArchiveSerializable, T>::value>>
  bool unarchive(ArchiveSerializable::ArchiveName name, T& archivable) {
    ArchiveDef<T> definition;
    return unarchiveInstance(name, definition.className(), archivable);
  }

 private:
  class Database;
  class Statement;
  class Transaction;

  friend class ArchiveItem;

  using Registrations = std::map<std::string, ArchiveSerializable::Members>;

  std::unique_ptr<Database> _db;
  size_t _transactionCount;
  Registrations _registrations;
  std::map<std::string, std::unique_ptr<Statement>> _insertStatements;
  std::map<std::string, std::unique_ptr<Statement>> _queryStatements;
  std::unique_ptr<Statement> _beginTransactionStatement;
  std::unique_ptr<Statement> _endTransactionStatement;

  Statement& cachedInsertStatement(const std::string& name, size_t members);
  Statement& cachedQueryStatement(const std::string& name, size_t members);

  void setupTransactionStatements();
  bool registerClass(const std::string& name,
                     const ArchiveSerializable::Members& members);
  bool archiveInstance(const std::string& className,
                       const ArchiveSerializable& archivable);
  bool unarchiveInstance(ArchiveSerializable::ArchiveName name,
                         const std::string& className,
                         ArchiveSerializable& archivable);

  RL_DISALLOW_COPY_AND_ASSIGN(Archive);
};

class ArchiveItem {
 public:
  bool encode(ArchiveSerializable::Member member, const std::string& item);
  bool encode(ArchiveSerializable::Member member, uint64_t item);
  bool encode(ArchiveSerializable::Member member, double item);
  bool encode(ArchiveSerializable::Member member, const Allocation& allocation);

  bool decode(ArchiveSerializable::Member member, std::string& item);
  bool decode(ArchiveSerializable::Member member, uint64_t& item);
  bool decode(ArchiveSerializable::Member member, double& item);
  bool decode(ArchiveSerializable::Member member, Allocation& allocation);

  ArchiveSerializable::ArchiveName name() const;

 private:
  ArchiveSerializable::ArchiveName _name;
  const ArchiveSerializable::Members& _members;
  Archive::Statement& _statement;

  friend class Archive;

  ArchiveItem(ArchiveSerializable::ArchiveName name,
              const ArchiveSerializable::Members& members,
              Archive::Statement& statement);

  RL_DISALLOW_COPY_AND_ASSIGN(ArchiveItem);
};

}  // namespace core
}  // namespace rl

#endif  // RADARLOVE_CORE_ARCHIVE_H_
