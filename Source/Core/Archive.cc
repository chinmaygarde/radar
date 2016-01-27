// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Archive.h>

#include <sqlite3/sqlite3.h>

#include <algorithm>
#include <sstream>

namespace rl {
namespace core {

static const char* ArchiveColumnPrefix = "item";
static const char* ArchivePrimaryKeyColumnName = "name";
static size_t ArchivePrimaryKeyIndex = 0;

class Archive::Statement {
 public:
  Statement(sqlite3* db, const std::string& statememt) : _statement(nullptr) {
    auto res = sqlite3_prepare_v2(db, statememt.c_str(),
                                  static_cast<int>(statememt.size()),
                                  &_statement, nullptr);
    _ready = res == SQLITE_OK && _statement != nullptr;
  }

  ~Statement() {
    auto res = sqlite3_finalize(_statement);
    RL_ASSERT(res == SQLITE_OK);
  }

  bool isReady() const { return _ready; }

  bool reset() {
    auto success = sqlite3_reset(_statement) == SQLITE_OK;
    success &= sqlite3_clear_bindings(_statement) == SQLITE_OK;
    return success;
  }

  static constexpr int ToParam(size_t index) {
    /*
     *  sqlite parameters begin from 1
     */
    return static_cast<int>(index + 1);
  }

  static constexpr int ToColumn(size_t index) {
    /*
     *  sqlite columns begin from 1
     */
    return static_cast<int>(index);
  }

  bool bind(size_t index, const std::string& item) {
    return sqlite3_bind_text(_statement,                     //
                             ToParam(index),                 //
                             item.data(),                    //
                             static_cast<int>(item.size()),  //
                             SQLITE_TRANSIENT) == SQLITE_OK;
  }

  bool bind(size_t index, uint64_t item) {
    return sqlite3_bind_int64(_statement,      //
                              ToParam(index),  //
                              item) == SQLITE_OK;
  }

  bool bind(size_t index, double item) {
    return sqlite3_bind_double(_statement,      //
                               ToParam(index),  //
                               item) == SQLITE_OK;
  }

  bool bind(size_t index, const Allocation& item) {
    return sqlite3_bind_blob(_statement,                     //
                             ToParam(index),                 //
                             item.data(),                    //
                             static_cast<int>(item.size()),  //
                             SQLITE_TRANSIENT) == SQLITE_OK;
  }

  bool column(size_t index, uint64_t& item) {
    item = sqlite3_column_int64(_statement, ToColumn(index));
    return true;
  }

  bool column(size_t index, double& item) {
    item = sqlite3_column_double(_statement, ToColumn(index));
    return true;
  }

  /*
   *  For cases where byte sizes of column data is necessary, the
   *  recommendations in https://www.sqlite.org/c3ref/column_blob.html regarding
   *  type conversions are followed.
   *
   *  TL;DR: Access blobs then bytes.
   */

  bool column(size_t index, std::string& item) {
    /*
     *  Get the character data
     */
    auto chars = reinterpret_cast<const char*>(
        sqlite3_column_text(_statement, ToColumn(index)));

    /*
     *  Get the length of the string (in bytes)
     */
    size_t textByteSize = sqlite3_column_bytes(_statement, ToColumn(index));

    std::string text(chars, textByteSize);
    item.swap(text);

    return true;
  }

  bool column(size_t index, Allocation& item) {
    /*
     *  Get a blob pointer
     */
    auto blob = reinterpret_cast<const uint8_t*>(
        sqlite3_column_blob(_statement, ToColumn(index)));

    /*
     *  Decode the number of bytes in the blob
     */
    size_t byteSize = sqlite3_column_bytes(_statement, ToColumn(index));

    /*
     *  Reszie the host allocation and move the blob contents into it
     */
    if (!item.resize(byteSize)) {
      return false;
    }

    memmove(item.data(), blob, byteSize);
    return true;
  }

  size_t columnCount() { return sqlite3_column_count(_statement); }

  enum class Result {
    Done,
    Row,
    Failure,
  };

  Result run() {
    switch (sqlite3_step(_statement)) {
      case SQLITE_DONE:
        return Result::Done;
      case SQLITE_ROW:
        return Result::Row;
      default:
        return Result::Failure;
    }
  }

 private:
  sqlite3_stmt* _statement;
  bool _ready;

  RL_DISALLOW_COPY_AND_ASSIGN(Statement);
};

class Archive::Transaction {
 public:
  Transaction(size_t& transactionCount,
              Statement& beginStatement,
              Statement& endStatement)
      : _transactionCount(transactionCount),
        _endStatement(endStatement),
        _cleanup(false) {
    if (_transactionCount == 0) {
      _cleanup = beginStatement.run() == Statement::Result::Done;
    }
    _transactionCount++;
  }

  ~Transaction() {
    if (_transactionCount == 1 && _cleanup) {
      auto res = _endStatement.run() == Statement::Result::Done;
      RL_ASSERT_MSG(res, "Must be able to commit the nested transaction");
    }
    _transactionCount--;
  }

 private:
  Statement& _endStatement;
  size_t& _transactionCount;
  bool _cleanup;

  RL_DISALLOW_COPY_AND_ASSIGN(Transaction);
};

class Archive::Database {
 public:
  Database(const std::string& filename) : _db(nullptr) {
    ::remove(filename.c_str());
    auto res = sqlite3_open(filename.c_str(), &_db);
    _ready = res == SQLITE_OK && _db != nullptr;
  }

  ~Database() {
    auto res = sqlite3_close(_db);
    RL_ASSERT(res == SQLITE_OK);
  }

  bool isReady() const { return _ready; }

  bool createTable(const std::string& name,
                   size_t columns,
                   bool autoIncrement) {
    if (!_ready || name.size() == 0 || columns == 0) {
      return false;
    }

    std::stringstream stream;

    /*
     *  Table names cannot participate in parameter substitution, so we prepare
     *  a statement and check its validity before running.
     */
    stream << "CREATE TABLE IF NOT EXISTS " << name.c_str() << " (";
    stream << ArchivePrimaryKeyColumnName;

    if (autoIncrement) {
      stream << " INTEGER PRIMARY KEY AUTOINCREMENT, ";
    } else {
      stream << " INTEGER UNIQUE PRIMARY KEY NOT NULL, ";
    }
    for (size_t i = 0; i < columns; i++) {
      stream << ArchiveColumnPrefix << std::to_string(i + 1);
      if (i != columns - 1) {
        stream << ", ";
      }
    }
    stream << ");";

    Statement statement(_db, stream.str().c_str());

    if (!statement.isReady()) {
      return false;
    }

    if (!statement.reset()) {
      return false;
    }

    return statement.run() == Statement::Result::Done;
  }

  sqlite3* handle() { return _db; };

 private:
  sqlite3* _db;
  bool _ready;

  RL_DISALLOW_COPY_AND_ASSIGN(Database);
};

Archive::Archive(const std::string& path)
    : _db(make_unique<Database>(path)), _transactionCount(0) {
  setupTransactionStatements();
}

Archive::~Archive() {}

void Archive::setupTransactionStatements() {
  if (!isReady()) {
    return;
  }

  _beginTransactionStatement =
      make_unique<Statement>(_db->handle(), "BEGIN TRANSACTION;");

  _endTransactionStatement =
      make_unique<Statement>(_db->handle(), "END TRANSACTION;");

  RL_ASSERT(_beginTransactionStatement->isReady() &&
            _endTransactionStatement->isReady());
}

bool Archive::registerDefinition(const ArchiveDef& definition) {
  auto found = _registrations.find(definition.className);
  if (found != _registrations.end()) {
    /*
     *  This class has already been registered
     */
    return false;
  }

  auto tableCreated = _db->createTable(definition.className,       //
                                       definition.members.size(),  //
                                       definition.autoAssignName);

  if (!tableCreated) {
    /*
     *  Could not create the table so there is no point in going forward
     */
    return false;
  }

  auto res = _registrations.emplace(definition.className);
  return res.second;
}

bool Archive::isReady() const {
  return _db->isReady();
}

Archive::Statement& Archive::cachedInsertStatement(
    const ArchiveDef& definition) {
  const auto& name = definition.className;
  const auto membersCount = definition.members.size();

  auto found = _insertStatements.find(definition.className);

  if (found != _insertStatements.end()) {
    return *(found->second);
  }

  std::stringstream stream;
  stream << "INSERT OR REPLACE INTO " << name << " VALUES ( ?, ";
  for (size_t i = 0; i < membersCount; i++) {
    stream << "?";
    if (i != membersCount - 1) {
      stream << ", ";
    }
  }
  stream << ");";

  auto inserted = _insertStatements.emplace(
      name, make_unique<Archive::Statement>(_db->handle(), stream.str()));
  RL_ASSERT(inserted.second);
  return *((*(inserted.first)).second);
}

Archive::Statement& Archive::cachedQueryStatement(const std::string& name,
                                                  size_t members) {
  auto found = _queryStatements.find(name);

  if (found != _queryStatements.end()) {
    return *(found->second);
  }

  std::stringstream stream;
  stream << "SELECT " << ArchivePrimaryKeyColumnName << ", ";
  for (size_t i = 0; i < members; i++) {
    stream << ArchiveColumnPrefix << std::to_string(i + 1);
    if (i != members - 1) {
      stream << ",";
    }
  }
  stream << " FROM " << name << " WHERE " << ArchivePrimaryKeyColumnName
         << " = ?;";

  auto inserted = _queryStatements.emplace(
      name, make_unique<Archive::Statement>(_db->handle(), stream.str()));
  RL_ASSERT(inserted.second);
  return *((*(inserted.first)).second);
}

bool Archive::archiveInstance(const ArchiveDef& definition,
                              const ArchiveSerializable& archivable) {
  if (!isReady()) {
    return false;
  }

  Transaction transaction(_transactionCount, *_beginTransactionStatement,
                          *_endTransactionStatement);

  const auto& className = definition.className;
  const auto& members = definition.members;

  auto registration = _registrations.find(className);

  if (registration == _registrations.end() && !registerDefinition(definition)) {
    return false;
  }

  auto& statement = cachedInsertStatement(definition);

  if (!statement.isReady() || !statement.reset()) {
    /*
     *  Must be able to reset the statement for a new write
     */
    return false;
  }

  /*
   *  The lifecycle of this item is tied to this scope and there is no way for
   *  the user to create an instance of an archive item. So pass the argument
   *  by reference.
   */
  auto itemName = archivable.archiveName();

  ArchiveItem item(*this, statement, members, itemName);

  if (!statement.bind(ArchivePrimaryKeyIndex, itemName)) {
    return false;
  }

  auto clientWrite = archivable.serialize(item);

  if (!clientWrite) {
    return false;
  }

  return statement.run() == Statement::Result::Done;
}

bool Archive::unarchiveInstance(const ArchiveDef& definition,
                                ArchiveSerializable::ArchiveName name,
                                ArchiveSerializable& archivable) {
  const auto& className = definition.className;
  const auto& members = definition.members;

  if (!isReady()) {
    return false;
  }

  auto registration = _registrations.find(className);

  if (registration == _registrations.end() && !registerDefinition(definition)) {
    return false;
  }

  auto membersCount = members.size();
  auto& statement = cachedQueryStatement(className, membersCount);

  if (!statement.isReady()) {
    return false;
  }

  if (!statement.bind(ArchivePrimaryKeyIndex, name)) {
    return false;
  }

  if (statement.run() != Statement::Result::Row) {
    return false;
  }

  auto columnCount = statement.columnCount();
  if (columnCount != membersCount + 1 /* primary key */) {
    return false;
  }

  ArchiveItem item(*this, statement, members, name);

  auto result = archivable.deserialize(item);

  statement.reset();

  return result;
}

ArchiveItem::ArchiveItem(Archive& context,
                         Archive::Statement& statement,
                         const ArchiveSerializable::Members& members,
                         ArchiveSerializable::ArchiveName name)
    : _context(context),
      _statement(statement),
      _members(members),
      _name(name) {}

ArchiveSerializable::ArchiveName ArchiveItem::name() const {
  return _name;
}

static std::pair<size_t, bool> IndexOfMember(
    const ArchiveSerializable::Members& members,
    ArchiveSerializable::Member member) {
  /*
   *  This should probably (and can easily be) optimized for constant time
   *  lookup. But the sizes of the arrays are extremely small.
   */
  auto found = std::find(members.begin(), members.end(), member);

  if (found == members.end()) {
    return {0, false};
  }

  return {std::distance(members.begin(), found) + 1 /* primary key */, true};
}

bool ArchiveItem::encode(ArchiveSerializable::Member member,
                         const std::string& item) {
  auto found = IndexOfMember(_members, member);
  return found.second ? _statement.bind(found.first, item) : false;
}

bool ArchiveItem::encodeIntegral(ArchiveSerializable::Member member,
                                 uint64_t item) {
  auto found = IndexOfMember(_members, member);
  return found.second ? _statement.bind(found.first, item) : false;
}

bool ArchiveItem::encode(ArchiveSerializable::Member member, double item) {
  auto found = IndexOfMember(_members, member);
  return found.second ? _statement.bind(found.first, item) : false;
}

bool ArchiveItem::encode(ArchiveSerializable::Member member,
                         const Allocation& item) {
  auto found = IndexOfMember(_members, member);
  return found.second ? _statement.bind(found.first, item) : false;
}

bool ArchiveItem::encode(ArchiveSerializable::Member member,
                         const ArchiveDef& otherDef,
                         const ArchiveSerializable& other) {
  auto found = IndexOfMember(_members, member);

  if (!found.second) {
    return false;
  }

  /*
   *  We need to fully archive the other instance first because it could
   *  have a name that is auto assigned. In that case, we cannot ask it before
   *  archival (via `other.archiveName()`).
   */
  if (!_context.archiveInstance(otherDef, other)) {
    return false;
  }

  /*
   *  Bind the name of the serialiable
   */
  if (!_statement.bind(found.first, other.archiveName() /* WIP: WRONG  */)) {
    return false;
  }

  return true;
}

bool ArchiveItem::decode(ArchiveSerializable::Member member,
                         std::string& item) {
  auto found = IndexOfMember(_members, member);
  return found.second ? _statement.column(found.first, item) : false;
}

bool ArchiveItem::decodeIntegral(ArchiveSerializable::Member member,
                                 uint64_t& item) {
  auto found = IndexOfMember(_members, member);
  return found.second ? _statement.column(found.first, item) : false;
}

bool ArchiveItem::decode(ArchiveSerializable::Member member, double& item) {
  auto found = IndexOfMember(_members, member);
  return found.second ? _statement.column(found.first, item) : false;
}

bool ArchiveItem::decode(ArchiveSerializable::Member member, Allocation& item) {
  auto found = IndexOfMember(_members, member);
  return found.second ? _statement.column(found.first, item) : false;
}

bool ArchiveItem::decode(ArchiveSerializable::Member member,
                         const ArchiveDef& otherDef,
                         ArchiveSerializable& other) {
  auto found = IndexOfMember(_members, member);
  if (found.second) {
    RL_ASSERT_MSG(false, "WIP");
    return false;
  } else {
    return false;
  }
}

}  // namespace core
}  // namespace rl
