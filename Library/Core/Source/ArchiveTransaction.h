/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>

namespace rl {
namespace core {

class ArchiveStatement;

class ArchiveTransaction {
 public:
  ArchiveTransaction(ArchiveTransaction&& transaction);

  ~ArchiveTransaction();

  void markWritesSuccessful();

 private:
  ArchiveStatement& _endStatement;
  ArchiveStatement& _rollbackStatement;
  int64_t& _transactionCount;
  bool _cleanup;
  bool _successful;
  bool _abandoned;

  friend class ArchiveDatabase;

  ArchiveTransaction(int64_t& transactionCount,
                     ArchiveStatement& beginStatement,
                     ArchiveStatement& endStatement,
                     ArchiveStatement& rollbackStatement);

  RL_DISALLOW_COPY_AND_ASSIGN(ArchiveTransaction);
};

}  // namespace core
}  // namespace rl
