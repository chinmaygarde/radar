// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Archive.h>

#include <sqlite3/sqlite3.h>

#include <sstream>

namespace rl {
namespace core {

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

  bool reset() { return sqlite3_reset(_statement) == SQLITE_OK; }

  bool bind(size_t index, const std::string& item) {
    return sqlite3_bind_text(_statement,                     //
                             static_cast<int>(index + 1),    //
                             item.data(),                    //
                             static_cast<int>(item.size()),  //
                             SQLITE_TRANSIENT) == SQLITE_OK;
  }

  bool bind(size_t index, uint64_t item) {
    return sqlite3_bind_int64(_statement,                   //
                              static_cast<int>(index + 1),  //
                              item) == SQLITE_OK;
  }

  bool bind(size_t index, double item) {
    return sqlite3_bind_double(_statement,                   //
                               static_cast<int>(index + 1),  //
                               item) == SQLITE_OK;
  }

  bool bind(size_t index, const Allocation& item) {
    return sqlite3_bind_blob(_statement,                     //
                             static_cast<int>(index + 1),    //
                             item.data(),                    //
                             static_cast<int>(item.size()),  //
                             SQLITE_TRANSIENT) == SQLITE_OK;
  }

  bool run() {
    if (!_ready) {
      return false;
    }

    auto res = sqlite3_step(_statement);
    return res == SQLITE_DONE;
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
      _cleanup = beginStatement.run();
    }
    _transactionCount++;
  }

  ~Transaction() {
    if (_transactionCount == 1 && _cleanup) {
      auto res = _endStatement.run();
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
    auto res = sqlite3_open(filename.c_str(), &_db);
    _ready = res == SQLITE_OK && _db != nullptr;
  }

  ~Database() {
    auto res = sqlite3_close(_db);
    RL_ASSERT(res == SQLITE_OK);
  }

  bool isReady() const { return _ready; }

  bool createTable(const std::string& name, size_t columns) {
    if (!_ready || name.size() == 0 || columns == 0) {
      return false;
    }

    std::stringstream stream;

    /*
     *  Table names cannot participate in parameter substitution, so we prepare
     *  a statement and check its validity before running.
     */
    stream << "CREATE TABLE IF NOT EXISTS " << name.c_str() << " (";
    for (size_t i = 0; i < columns; i++) {
      stream << "column_" << std::to_string(i + 1);
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

    return statement.run();
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

bool Archive::registerClass(const std::string& name,
                            const Archivable::Members& members) {
  auto found = _registrations.find(name);
  if (found != _registrations.end()) {
    /*
     *  This class has already been registered
     */
    return false;
  }

  auto tableCreated = _db->createTable(name, members.size());

  if (!tableCreated) {
    /*
     *  Could not create the table so there is no point in going forward
     */
    return false;
  }

  _registrations[name] = members;
  return true;
}

bool Archive::isReady() const {
  return _db->isReady();
}

std::unique_ptr<Archive::Statement>& Archive::cachedInsertStatement(
    const std::string& name,
    size_t cols) {
  auto found = _insertStatements.find(name);

  if (found != _insertStatements.end()) {
    /*
     *  The statement has been found in the cache
     */
    return found->second;
  }

  std::stringstream stream;
  stream << "INSERT INTO " << name << " VALUES (";
  for (size_t i = 0; i < cols; i++) {
    stream << "?";
    if (i != cols - 1) {
      stream << ",";
    }
  }
  stream << ");";

  auto inserted = _insertStatements.emplace(
      name, make_unique<Archive::Statement>(_db->handle(), stream.str()));
  RL_ASSERT(inserted.second);
  return (*(inserted.first)).second;
}

bool Archive::archiveClass(const std::string& className,
                           const Archivable& archivable) {
  if (!isReady()) {
    return false;
  }

  Transaction transaction(_transactionCount, *_beginTransactionStatement,
                          *_endTransactionStatement);

  auto found = _registrations.find(className);

  if (found == _registrations.end()) {
    /*
     *  There were no registrations for this class
     */
    return false;
  }

  auto& statement =
      *(cachedInsertStatement(found->first, found->second.size()));

  if (!statement.reset()) {
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
  ArchiveItem item(found->second, statement);

  auto clientWrite = archivable.writeToArchive(item);

  if (!clientWrite) {
    return false;
  }

  return statement.run();
}

ArchiveItem::ArchiveItem(const Archivable::Members& members,
                         Archive::Statement& statement)
    : _members(members), _statement(statement) {}

static std::pair<size_t, bool> IndexOfMember(const Archivable::Members& members,
                                             Archivable::Member member) {
  /*
   *  This should probably (and can easily be) optimized for constant time
   *  lookup. But the sizes of the arrays are extremely small.
   */
  auto found = std::find(members.begin(), members.end(), member);

  if (found == members.end()) {
    return {0, false};
  }

  return {std::distance(members.begin(), found), true};
}

bool ArchiveItem::encode(Archivable::Member member, const std::string& item) {
  auto found = IndexOfMember(_members, member);
  return found.second ? _statement.bind(found.first, item) : false;
}

bool ArchiveItem::encode(Archivable::Member member, uint64_t item) {
  auto found = IndexOfMember(_members, member);
  return found.second ? _statement.bind(found.first, item) : false;
}

bool ArchiveItem::encode(Archivable::Member member, double item) {
  auto found = IndexOfMember(_members, member);
  return found.second ? _statement.bind(found.first, item) : false;
}

bool ArchiveItem::encode(Archivable::Member member, const Allocation& item) {
  auto found = IndexOfMember(_members, member);
  return found.second ? _statement.bind(found.first, item) : false;
}

}  // namespace core
}  // namespace rl
