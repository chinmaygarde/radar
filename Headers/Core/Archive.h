// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_CORE_ARCHIVE_H_
#define RADARLOVE_CORE_ARCHIVE_H_

#include <Core/Allocation.h>
#include <Core/Macros.h>
#include <Core/Utilities.h>

#include <set>
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

struct ArchiveDef {
  const bool autoAssignName;
  const std::string className;
  const ArchiveSerializable::Members members;
};

static const ArchiveSerializable::ArchiveName ArchiveNameAuto = 0;

class Archive {
 public:
  Archive(const std::string& path);

  ~Archive();

  bool isReady() const;

  template <class T,
            class = only_if<std::is_base_of<ArchiveSerializable, T>::value>>
  bool archive(const T& archivable) {
    const ArchiveDef& def = T::ArchiveDefinition;
    int64_t unusedLast = 0;
    return archiveInstance(def, archivable, unusedLast);
  }

  template <class T,
            class = only_if<std::is_base_of<ArchiveSerializable, T>::value>>
  bool unarchive(ArchiveSerializable::ArchiveName name, T& archivable) {
    const ArchiveDef& def = T::ArchiveDefinition;
    return unarchiveInstance(def, name, archivable);
  }

 private:
  class Database;
  class Statement;
  class Transaction;

  friend class ArchiveItem;

  std::unique_ptr<Database> _db;
  size_t _transactionCount;
  std::set<std::string> _registrations;
  std::map<std::string, std::unique_ptr<Statement>> _insertStatements;
  std::map<std::string, std::unique_ptr<Statement>> _queryStatements;
  std::unique_ptr<Statement> _beginTransactionStatement;
  std::unique_ptr<Statement> _endTransactionStatement;

  Statement& cachedInsertStatement(const ArchiveDef& definition);
  Statement& cachedQueryStatement(const std::string& name, size_t members);

  void setupTransactionStatements();
  bool registerDefinition(const ArchiveDef& definition);
  bool archiveInstance(const ArchiveDef& definition,
                       const ArchiveSerializable& archivable,
                       int64_t& lastInsertID);
  bool unarchiveInstance(const ArchiveDef& definition,
                         ArchiveSerializable::ArchiveName name,
                         ArchiveSerializable& archivable);

  RL_DISALLOW_COPY_AND_ASSIGN(Archive);
};

class ArchiveVector : public ArchiveSerializable {
 public:
  static const ArchiveDef ArchiveDefinition;

  ArchiveName archiveName() const override;

  bool serialize(ArchiveItem& item) const override;

  bool deserialize(ArchiveItem& item) override;

 private:
  std::vector<int64_t> _keys;

  friend class ArchiveItem;

  ArchiveVector(std::vector<int64_t>&& keys);

  RL_DISALLOW_COPY_AND_ASSIGN(ArchiveVector);
};

class ArchiveItem {
 public:
  template <class T, class = only_if<std::is_integral<T>::value>>
  bool encode(ArchiveSerializable::Member member, T item) {
    return encodeIntegral(member, static_cast<int64_t>(item));
  }

  bool encode(ArchiveSerializable::Member member, double item);
  bool encode(ArchiveSerializable::Member member, const std::string& item);
  bool encode(ArchiveSerializable::Member member, const Allocation& allocation);

  template <class T,
            class = only_if<std::is_base_of<ArchiveSerializable, T>::value>>
  bool encodeArchivable(ArchiveSerializable::Member member, const T& other) {
    const ArchiveDef& otherDef = T::ArchiveDefinition;
    return encode(member, otherDef, other);
  }

  template <class T, class = only_if<std::is_enum<T>::value>>
  bool encodeEnum(ArchiveSerializable::Member member, const T& item) {
    return encodeIntegral(member, static_cast<int64_t>(item));
  }

  template <class T,
            class = only_if<std::is_base_of<ArchiveSerializable, T>::value>>
  bool encode(ArchiveSerializable::Member member, const std::vector<T>& items) {
    /*
     *  All items in the vector are individually encoded and their keys noted
     */
    std::vector<int64_t> members;
    members.reserve(items.size());

    const ArchiveDef& itemDefinition = T::ArchiveDefinition;
    for (const auto& item : items) {
      int64_t added = 0;
      bool result = _context.archiveInstance(itemDefinition, item, added);
      if (!result) {
        return false;
      }
      members.emplace_back(added);
    }

    /*
     *  The keys are flattened into the vectors table. Write to that table
     */
    ArchiveVector vector(std::move(members));
    int64_t vectorID = 0;
    if (!_context.archiveInstance(ArchiveVector::ArchiveDefinition,  //
                                  vector,                            //
                                  vectorID)) {
      return false;
    }

    return encodeIntegral(member, vectorID);
  }

  template <class T, class = only_if<std::is_integral<T>::value>>
  bool decode(ArchiveSerializable::Member member, T& item) {
    int64_t decoded = 0;
    auto result = decodeIntegral(member, decoded);
    item = static_cast<T>(decoded);
    return result;
  }

  bool decode(ArchiveSerializable::Member member, double& item);
  bool decode(ArchiveSerializable::Member member, std::string& item);
  bool decode(ArchiveSerializable::Member member, Allocation& allocation);

  template <class T,
            class = only_if<std::is_base_of<ArchiveSerializable, T>::value>>
  bool decodeArchivable(ArchiveSerializable::Member member, T& other) {
    const ArchiveDef& otherDef = T::ArchiveDefinition;
    return decode(member, otherDef, other);
  }

  template <class T, class = only_if<std::is_enum<T>::value>>
  bool decodeEnum(ArchiveSerializable::Member member, T& item) {
    int64_t desugared = 0;
    if (decodeIntegral(member, desugared)) {
      item = static_cast<T>(desugared);
      return true;
    }
    return false;
  }

  template <class T,
            class = only_if<std::is_base_of<ArchiveSerializable, T>::value>>
  bool decode(ArchiveSerializable::Member member, std::vector<T>& item) {
    /*
     *  WIP
     */
    return true;
  }

  ArchiveSerializable::ArchiveName name() const;

 private:
  Archive& _context;
  Archive::Statement& _statement;
  const ArchiveSerializable::Members& _members;
  ArchiveSerializable::ArchiveName _name;

  friend class Archive;

  ArchiveItem(Archive& context,
              Archive::Statement& statement,
              const ArchiveSerializable::Members& members,
              ArchiveSerializable::ArchiveName name);

  bool encodeIntegral(ArchiveSerializable::Member member, int64_t item);
  bool decodeIntegral(ArchiveSerializable::Member member, int64_t& item);

  bool encode(ArchiveSerializable::Member member,
              const ArchiveDef& otherDef,
              const ArchiveSerializable& other);
  bool decode(ArchiveSerializable::Member member,
              const ArchiveDef& otherDef,
              ArchiveSerializable& other);

  RL_DISALLOW_COPY_AND_ASSIGN(ArchiveItem);
};

}  // namespace core
}  // namespace rl

#endif  // RADARLOVE_CORE_ARCHIVE_H_
