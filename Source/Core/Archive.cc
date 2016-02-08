// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Archive.h>
#include <Core/ArchiveClassRegistration.h>
#include <Core/ArchiveVector.h>
#include <Core/ArchiveDatabase.h>
#include <Core/ArchiveStatement.h>

namespace rl {
namespace core {

Archive::Archive(const std::string& path)
    : _db(make_unique<ArchiveDatabase>(path)), _transactionCount(0) {}

Archive::~Archive() {}

bool Archive::isReady() const {
  return _db->isReady();
}

bool Archive::archiveInstance(const ArchiveDef& definition,
                              const ArchiveSerializable& archivable,
                              int64_t& lastInsertIDOut) {
  if (!isReady()) {
    return false;
  }

  auto transaction = _db->acquireTransaction(_transactionCount);

  const auto* registration = _db->registrationForDefinition(definition);

  if (registration == nullptr) {
    return false;
  }

  auto statement = registration->insertStatement();

  if (!statement.isReady()) {
    /*
     *  Must be able to reset the statement for a new write
     */
    return false;
  }

  /*
   *  The lifecycle of the archive item is tied to this scope and there is no
   *  way for the user to create an instance of an archive item. So its safe
   *  for its members to be references. It does not manage the lifetimes of
   *  anything.
   */

  auto itemName = archivable.archiveName();

  ArchiveItem item(*this, statement, *registration, itemName);

  /*
   *  We need to bind the primary key only if the item does not provide its own
   */
  if (!definition.autoAssignName &&
      !statement.bind(ArchiveClassRegistration::NameIndex, itemName)) {
    return false;
  }

  if (!archivable.serialize(item)) {
    return false;
  }

  if (statement.run() != ArchiveStatement::Result::Done) {
    return false;
  }

  auto lastInsert = _db->lastInsertRowID();

  if (!definition.autoAssignName && lastInsert != itemName) {
    return false;
  }

  lastInsertIDOut = lastInsert;

  /*
   *  If any of the nested calls fail, we would have already checked for the
   *  failure and returned.
   */
  transaction.markWritesSuccessful();

  return true;
}

bool Archive::unarchiveInstance(const ArchiveDef& definition,
                                ArchiveSerializable::ArchiveName name,
                                ArchiveSerializable& archivable,
                                Namespace* ns) {
  if (!isReady()) {
    return false;
  }

  const auto* registration = _db->registrationForDefinition(definition);

  if (registration == nullptr) {
    return false;
  }

  auto statement = registration->queryStatement();

  if (!statement.isReady()) {
    return false;
  }

  if (!statement.bind(ArchiveClassRegistration::NameIndex, name)) {
    return false;
  }

  if (statement.run() != ArchiveStatement::Result::Row) {
    return false;
  }

  if (statement.columnCount() !=
      registration->memberCount() + 1 /* primary key */) {
    return false;
  }

  ArchiveItem item(*this, statement, *registration, name);

  auto result = archivable.deserialize(item, ns);

  statement.reset();

  return result;
}

ArchiveItem::ArchiveItem(Archive& context,
                         ArchiveStatement& statement,
                         const ArchiveClassRegistration& registration,
                         ArchiveSerializable::ArchiveName name)
    : _context(context),
      _statement(statement),
      _registration(registration),
      _name(name),
      _currentClass(registration.className()) {}

ArchiveSerializable::ArchiveName ArchiveItem::name() const {
  return _name;
}

bool ArchiveItem::encode(ArchiveSerializable::Member member,
                         const std::string& item) {
  auto found = _registration.findColumn(_currentClass, member);
  return found.second ? _statement.bind(found.first, item) : false;
}

bool ArchiveItem::encodeIntegral(ArchiveSerializable::Member member,
                                 int64_t item) {
  auto found = _registration.findColumn(_currentClass, member);
  return found.second ? _statement.bind(found.first, item) : false;
}

bool ArchiveItem::encode(ArchiveSerializable::Member member, double item) {
  auto found = _registration.findColumn(_currentClass, member);
  return found.second ? _statement.bind(found.first, item) : false;
}

bool ArchiveItem::encode(ArchiveSerializable::Member member,
                         const Allocation& item) {
  auto found = _registration.findColumn(_currentClass, member);
  return found.second ? _statement.bind(found.first, item) : false;
}

bool ArchiveItem::encode(ArchiveSerializable::Member member,
                         const ArchiveDef& otherDef,
                         const ArchiveSerializable& other) {
  auto found = _registration.findColumn(_currentClass, member);

  if (!found.second) {
    return false;
  }

  /*
   *  We need to fully archive the other instance first because it could
   *  have a name that is auto assigned. In that case, we cannot ask it before
   *  archival (via `other.archiveName()`).
   */
  int64_t lastInsert = 0;
  if (!_context.archiveInstance(otherDef, other, lastInsert)) {
    return false;
  }

  /*
   *  Bind the name of the serialiable
   */
  if (!_statement.bind(found.first, lastInsert)) {
    return false;
  }

  return true;
}

std::pair<bool, int64_t> ArchiveItem::encodeVectorKeys(
    std::vector<int64_t>&& members) {
  ArchiveVector vector(std::move(members));
  int64_t vectorID = 0;
  if (!_context.archiveInstance(ArchiveVector::ArchiveDefinition,  //
                                vector,                            //
                                vectorID)) {
    return {false, 0};
  }
  return {true, vectorID};
}

bool ArchiveItem::decode(ArchiveSerializable::Member member,
                         std::string& item) {
  auto found = _registration.findColumn(_currentClass, member);
  return found.second ? _statement.column(found.first, item) : false;
}

bool ArchiveItem::decodeIntegral(ArchiveSerializable::Member member,
                                 int64_t& item) {
  auto found = _registration.findColumn(_currentClass, member);
  return found.second ? _statement.column(found.first, item) : false;
}

bool ArchiveItem::decode(ArchiveSerializable::Member member, double& item) {
  auto found = _registration.findColumn(_currentClass, member);
  return found.second ? _statement.column(found.first, item) : false;
}

bool ArchiveItem::decode(ArchiveSerializable::Member member, Allocation& item) {
  auto found = _registration.findColumn(_currentClass, member);
  return found.second ? _statement.column(found.first, item) : false;
}

bool ArchiveItem::decode(ArchiveSerializable::Member member,
                         const ArchiveDef& otherDef,
                         ArchiveSerializable& other) {
  auto found = _registration.findColumn(_currentClass, member);
  if (found.second) {
    RL_ASSERT_MSG(false, "WIP");
    return false;
  } else {
    return false;
  }
}

}  // namespace core
}  // namespace rl
