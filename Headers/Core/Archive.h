// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_CORE_ARCHIVE_H_
#define RADARLOVE_CORE_ARCHIVE_H_

#include <Core/Macros.h>
#include <Core/Allocation.h>
#include <Core/Utilities.h>

#include <string>
#include <vector>
#include <map>

namespace rl {
namespace core {

class ArchiveItem;

class Archivable {
 public:
  using Member = uint64_t;
  using Members = std::vector<Member>;
  using PrimaryKey = uint64_t;

  virtual PrimaryKey archiveName() const = 0;

  virtual bool writeToArchive(ArchiveItem& item) const = 0;
  virtual bool readFromArchive(ArchiveItem& item) = 0;
};

template <class T>
class ArchiveDef {
 public:
  std::string className();
  Archivable::Members members();
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

  template <class T, class = only_if<std::is_base_of<Archivable, T>::value>>
  bool archive(const T& archivable) {
    ArchiveDef<T> definition;
    return archiveClass(definition.className(), archivable);
  }

 private:
  class Database;
  class Statement;
  class Transaction;

  friend class ArchiveItem;

  std::unique_ptr<Database> _db;
  size_t _transactionCount;
  std::map<std::string, Archivable::Members> _registrations;
  std::map<std::string, std::unique_ptr<Statement>> _insertStatements;
  std::unique_ptr<Statement> _beginTransactionStatement;
  std::unique_ptr<Statement> _endTransactionStatement;

  std::unique_ptr<Statement>& cachedInsertStatement(const std::string& name,
                                                    size_t cols);
  void setupTransactionStatements();
  bool registerClass(const std::string& name,
                     const Archivable::Members& members);
  bool archiveClass(const std::string& className, const Archivable& archivable);

  RL_DISALLOW_COPY_AND_ASSIGN(Archive);
};

class ArchiveItem {
 public:
  bool encode(Archivable::Member member, const std::string& item);
  bool encode(Archivable::Member member, uint64_t item);
  bool encode(Archivable::Member member, double item);
  bool encode(Archivable::Member member, const Allocation& allocation);

  bool decode(Archivable::Member member, std::string& item);
  bool decode(Archivable::Member member, uint64_t& item);
  bool decode(Archivable::Member member, double& item);
  bool decode(Archivable::Member member, Allocation& allocation);

 private:
  Archivable::PrimaryKey _primaryKey;
  const Archivable::Members& _members;
  Archive::Statement& _statement;
  bool _ready;

  friend class Archive;

  bool isReady() const;

  ArchiveItem(Archivable::PrimaryKey primaryKey,
              const Archivable::Members& members,
              Archive::Statement& statement);

  RL_DISALLOW_COPY_AND_ASSIGN(ArchiveItem);
};

}  // namespace core
}  // namespace rl

#endif  // RADARLOVE_CORE_ARCHIVE_H_
