/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Allocation.h>
#include <Core/Macros.h>
#include <Core/Namespace.h>
#include <Core/Utilities.h>
#include <string>
#include <vector>

namespace rl {
namespace core {

class ArchiveItem;
class ArchiveClassRegistration;
class ArchiveDatabase;
class ArchiveStatement;

class ArchiveSerializable {
 public:
  using Member = uint64_t;
  using Members = std::vector<Member>;
  using ArchiveName = uint64_t;

  virtual ArchiveName archiveName() const = 0;
  virtual bool serialize(ArchiveItem& item) const = 0;
  virtual bool deserialize(ArchiveItem& item, Namespace* ns) = 0;
};

struct ArchiveDef {
  const ArchiveDef* superClass;
  const std::string className;
  const bool autoAssignName;
  const ArchiveSerializable::Members members;
};

static const ArchiveSerializable::ArchiveName ArchiveNameAuto = 0;

class Archive {
 public:
  Archive(const std::string& path, bool recreate);

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
  bool unarchive(ArchiveSerializable::ArchiveName name,
                 T& archivable,
                 Namespace* ns) {
    const ArchiveDef& def = T::ArchiveDefinition;
    return unarchiveInstance(def, name, archivable, ns);
  }

  using UnarchiveStep = std::function<bool /*continue*/ (ArchiveItem&)>;

  template <class T,
            class = only_if<std::is_base_of<ArchiveSerializable, T>::value>>
  size_t unarchive(UnarchiveStep stepper) {
    const ArchiveDef& def = T::ArchiveDefinition;
    return unarchiveInstances(def, stepper, ArchiveNameAuto);
  }

 private:
  std::unique_ptr<ArchiveDatabase> _db;
  int64_t _transactionCount;

  friend class ArchiveItem;

  bool archiveInstance(const ArchiveDef& definition,
                       const ArchiveSerializable& archivable,
                       int64_t& lastInsertID);
  bool unarchiveInstance(const ArchiveDef& definition,
                         ArchiveSerializable::ArchiveName name,
                         ArchiveSerializable& archivable,
                         Namespace* ns);
  size_t unarchiveInstances(const ArchiveDef& definition,
                            UnarchiveStep stepper,
                            ArchiveSerializable::ArchiveName optionalName);

  RL_DISALLOW_COPY_AND_ASSIGN(Archive);
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

  bool encode(ArchiveSerializable::Member member, const Name& name);

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
    auto vectorInsert = encodeVectorKeys(std::move(members));

    if (!vectorInsert.first) {
      return false;
    }

    return encodeIntegral(member, vectorInsert.second);
  }

  template <
      class Super,
      class Current,
      class = only_if<std::is_base_of<ArchiveSerializable, Super>::value &&
                      std::is_base_of<ArchiveSerializable, Current>::value>>
  bool encodeSuper(const Current& thiz) {
    std::string oldClass = _currentClass;
    _currentClass = Super::ArchiveDefinition.className;
    auto success = thiz.Super::serialize(*this);
    _currentClass = oldClass;
    return success;
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

  bool decode(ArchiveSerializable::Member member, Name& name, Namespace* ns);

  template <class T,
            class = only_if<std::is_base_of<ArchiveSerializable, T>::value>>
  bool decodeArchivable(ArchiveSerializable::Member member,
                        T& other,
                        Namespace* ns) {
    const ArchiveDef& otherDef = T::ArchiveDefinition;
    return decode(member, otherDef, other, ns);
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
  bool decode(ArchiveSerializable::Member member,
              std::vector<T>& items,
              Namespace* ns) {
    /*
     *  From the member, find the foreign key of the vector
     */
    int64_t vectorForeignKey = 0;
    if (!decodeIntegral(member, vectorForeignKey)) {
      return false;
    }

    /*
     *  Get vector keys
     */
    std::vector<int64_t> keys;
    if (!decodeVectorKeys(vectorForeignKey, keys)) {
      return false;
    }

    const ArchiveDef& otherDef = T::ArchiveDefinition;
    for (const auto& key : keys) {
      items.emplace_back();

      if (!_context.unarchiveInstance(otherDef, key, items.back(), ns)) {
        return false;
      }
    }

    return true;
  }

  template <
      class Super,
      class Current,
      class = only_if<std::is_base_of<ArchiveSerializable, Super>::value &&
                      std::is_base_of<ArchiveSerializable, Current>::value>>
  bool decodeSuper(Current& thiz, Namespace* ns) {
    std::string oldClass = _currentClass;
    _currentClass = Super::ArchiveDefinition.className;
    auto success = thiz.Super::deserialize(*this, ns);
    _currentClass = oldClass;
    return success;
  }

  ArchiveSerializable::ArchiveName name() const;

 private:
  Archive& _context;
  ArchiveStatement& _statement;
  const ArchiveClassRegistration& _registration;
  ArchiveSerializable::ArchiveName _name;
  std::string _currentClass;

  friend class Archive;

  ArchiveItem(Archive& context,
              ArchiveStatement& statement,
              const ArchiveClassRegistration& registration,
              ArchiveSerializable::ArchiveName name);

  bool encodeIntegral(ArchiveSerializable::Member member, int64_t item);
  bool decodeIntegral(ArchiveSerializable::Member member, int64_t& item);

  std::pair<bool, int64_t> encodeVectorKeys(std::vector<int64_t>&& members);
  bool decodeVectorKeys(ArchiveSerializable::ArchiveName name,
                        std::vector<int64_t>& members);

  bool encode(ArchiveSerializable::Member member,
              const ArchiveDef& otherDef,
              const ArchiveSerializable& other);
  bool decode(ArchiveSerializable::Member member,
              const ArchiveDef& otherDef,
              ArchiveSerializable& other,
              Namespace* ns);

  RL_DISALLOW_COPY_AND_ASSIGN(ArchiveItem);
};

}  // namespace core
}  // namespace rl
