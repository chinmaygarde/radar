// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_CORE_ARCHIVETRANSACTION_H_
#define RADARLOVE_CORE_ARCHIVETRANSACTION_H_

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

#endif  // RADARLOVE_CORE_ARCHIVETRANSACTION_H_