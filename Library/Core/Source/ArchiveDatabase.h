/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <map>
#include "ArchiveTransaction.h"

namespace rl {
namespace core {

class ArchiveStatement;
class ArchiveClassRegistration;
struct ArchiveDef;

class ArchiveDatabase {
 public:
  ArchiveDatabase(const std::string& filename, bool recreate);

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
