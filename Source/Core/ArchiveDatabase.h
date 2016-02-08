// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_CORE_ARCHIVEDATABASE_H_
#define RADARLOVE_CORE_ARCHIVEDATABASE_H_

#include <Core/Macros.h>
#include <map>

#include <Core/ArchiveTransaction.h>

namespace rl {
namespace core {

class ArchiveStatement;
class ArchiveClassRegistration;
class ArchiveDef;

class ArchiveDatabase {
 public:
  ArchiveDatabase(const std::string& filename);

  ~ArchiveDatabase();

  bool isReady() const;

  int64_t lastInsertRowID();

  const ArchiveClassRegistration* registrationForDefinition(
      const ArchiveDef& definition);

  ArchiveTransaction acquireTransaction(int64_t& transactionCount);

 private:
  void* _db;
  bool _ready;
  std::map<std::string, std::unique_ptr<ArchiveClassRegistration>>
      _registrations;
  std::unique_ptr<ArchiveStatement> _beginTransaction;
  std::unique_ptr<ArchiveStatement> _endTransaction;
  std::unique_ptr<ArchiveStatement> _rollbackTransaction;

  friend class ArchiveClassRegistration;

  ArchiveStatement acquireStatement(const std::string& statementString) const;

  RL_DISALLOW_COPY_AND_ASSIGN(ArchiveDatabase);
};

}  // namespace core
}  // namespace rl

#endif  // RADARLOVE_CORE_ARCHIVEDATABASE_H_
